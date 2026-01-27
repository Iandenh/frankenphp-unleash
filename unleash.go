package unleash

// #include <stdlib.h>
// #include "unleash.h"
import "C"
import (
	"time"
	"unsafe"

	"fmt"
	"net/http"
	"sync"

	"github.com/Unleash/unleash-go-sdk/v5"
	"github.com/Unleash/unleash-go-sdk/v5/context"
	"github.com/dunglas/frankenphp"
)

func init() {
	frankenphp.RegisterExtension(unsafe.Pointer(&C.unleash_module_entry))
}

var (
	loadLock       sync.RWMutex
	clientRegistry = make(map[string]*unleash.Client)
)

func toContext(input map[string]any) context.Context {
	// Initialize the context with an empty map for properties
	ctx := context.Context{
		Properties: make(map[string]string),
	}

	for k, v := range input {
		if k == "properties" {
			properties, ok := v.(map[string]string)

			if ok {
				ctx.Properties = properties
			}
			continue
		}

		strVal, ok := v.(string)
		if !ok {
			strVal = fmt.Sprint(v)
		}

		switch k {
		case "userId", "UserId":
			ctx.UserId = strVal
		case "sessionId", "SessionId":
			ctx.SessionId = strVal
		case "remoteAddress", "RemoteAddress":
			ctx.RemoteAddress = strVal
		case "environment", "Environment":
			ctx.Environment = strVal
		case "appName", "AppName":
			ctx.AppName = strVal
		case "currentTime", "CurrentTime":
			ctx.CurrentTime = strVal
		default:
			ctx.Properties[k] = strVal
		}
	}

	return ctx
}

//export go_client_create
func go_client_create(
	phpName, phpUpstream, phpToken *C.zend_string,
	phpDisableMetrics, phpSteamingMode, phpDebugMode bool,
	phpRefreshInterval, phpMetricInterval int64,
) *C.char {
	name := zendStringToGoString(phpName)

	loadLock.RLock()

	_, ok := clientRegistry[name]
	if ok {
		loadLock.RUnlock()
		return nil
	}

	loadLock.RUnlock()
	loadLock.Lock()
	defer loadLock.Unlock()

	options := []unleash.ConfigOption{
		unleash.WithAppName(name),
		unleash.WithUrl(zendStringToGoString(phpUpstream)),
		unleash.WithRefreshInterval(time.Duration(phpRefreshInterval) * time.Millisecond),
		unleash.WithMetricsInterval(time.Duration(phpMetricInterval) * time.Millisecond),
	}

	token := zendStringToGoString(phpToken)

	if token != "" {
		options = append(options, unleash.WithCustomHeaders(http.Header{"Authorization": {token}}))
	}

	if phpSteamingMode == true {
		options = append(options, unleash.WithExperimentalMode(map[string]string{
			"type": "streaming",
		}))
	}

	if phpDebugMode == true {
		options = append(options, unleash.WithListener(&unleash.DebugListener{}))
	}

	if phpDisableMetrics == true {
		options = append(options, unleash.WithDisableMetrics(true))
	}

	client, err := unleash.NewClient(options...)

	if err != nil {
		return C.CString(err.Error())
	}

	done := make(chan bool, 1)

	go func() {
		client.WaitForReady() // Blocks here
		done <- true
	}()

	select {
	case <-done:
		clientRegistry[name] = client

		return nil
	case <-time.After(5 * time.Second):
		client.Close()

		return C.CString("Timeout: Client took too long to get ready")
	}
}

//export go_client_is_enabled
func go_client_is_enabled(phpName, phpFeatureFlag *C.zend_string, ctx *C.zend_array) (bool, *C.char) {
	name := zendStringToGoString(phpName)
	str := zendStringToGoString(phpFeatureFlag)

	loadLock.RLock()
	defer loadLock.RUnlock()

	client, ok := clientRegistry[name]

	if !ok {
		return false, C.CString("client not loaded")
	}

	var featureOptions []unleash.FeatureOption

	if ctx != nil {
		array, err := frankenphp.GoMap[any](unsafe.Pointer(ctx))

		if err != nil {
			return false, C.CString(fmt.Sprintf("error on context: %s", err))
		}

		if len(array) > 0 {
			featureOptions = append(featureOptions, unleash.WithContext(toContext(array)))
		}
	}

	return client.IsEnabled(str, featureOptions...), nil
}

//export go_client_close
func go_client_close(phpName *C.zend_string) *C.char {
	name := zendStringToGoString(phpName)

	loadLock.Lock()
	defer loadLock.Unlock()

	client, ok := clientRegistry[name]

	if !ok {
		return nil
	}

	err := client.Close()

	if err != nil {
		return C.CString(err.Error())
	}

	delete(clientRegistry, name)

	return nil
}

func zendStringToGoString(zendStr *C.zend_string) string {
	if zendStr == nil {
		return ""
	}

	return C.GoStringN((*C.char)(unsafe.Pointer(&zendStr.val)), C.int(zendStr.len))
}
