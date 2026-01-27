#include <php.h>
#include <Zend/zend_API.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_types.h>
#include <zend_exceptions.h>
#include <stddef.h>

#include "unleash.h"
#include "unleash_arginfo.h"
#include "_cgo_export.h"


zend_class_entry *unleash_ce;
static zend_object_handlers unleash_object_handlers;

typedef struct _unleash_client_object {
    zend_string *client_name;
    zend_object std;
} unleash_client_object;

static inline unleash_client_object *unleash_client_from_obj(zend_object *obj) {
    return (unleash_client_object*)((char *)(obj) - XtOffsetOf(unleash_client_object, std));
}

static void unleash_client_free(zend_object *object) {
    unleash_client_object *intern = unleash_client_from_obj(object);

    if (intern->client_name) {
        zend_string_release(intern->client_name);
        intern->client_name = NULL;
    }

    zend_object_std_dtor(&intern->std);
}

static zend_object *unleash_client_create(zend_class_entry *class_type) {
    unleash_client_object *intern = zend_object_alloc(sizeof(unleash_client_object), class_type);

    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);

    intern->std.handlers = &unleash_object_handlers;
    intern->client_name = NULL;

    return &intern->std;
}

static zend_function *unleash_get_constructor(zend_object *object) {
    zend_throw_error(NULL, "Cannot directly construct \\Iandenh\\Unleash\\Client, "
                     "use the getOrCreate() method instead");

    return NULL;
}

PHP_MINIT_FUNCTION(unleash) {
    unleash_ce = register_class_Iandenh_Unleash_Client();
    unleash_ce->create_object = unleash_client_create;

    memcpy(&unleash_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));

    unleash_object_handlers.offset = XtOffsetOf(unleash_client_object, std);
    unleash_object_handlers.free_obj = unleash_client_free;
    unleash_object_handlers.get_constructor = unleash_get_constructor;
    unleash_object_handlers.clone_obj = NULL;

    return SUCCESS;
}

PHP_METHOD(Iandenh_Unleash_Client, getOrCreate) {
    zend_string *name = NULL, *upstream = NULL, *token = NULL;
    bool disable_metrics = false, streaming_mode = false, debug_mode = false;
    zend_long refresh_interval_milliseconds = 15000, metrics_interval_milliseconds = 60000;

    ZEND_PARSE_PARAMETERS_START(3, 8)
        Z_PARAM_STR(name)
        Z_PARAM_STR(upstream)
        Z_PARAM_STR(token)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(disable_metrics)
        Z_PARAM_BOOL(streaming_mode)
        Z_PARAM_BOOL(debug_mode)
        Z_PARAM_LONG(refresh_interval_milliseconds)
        Z_PARAM_LONG(metrics_interval_milliseconds)
    ZEND_PARSE_PARAMETERS_END();

    char *error = go_client_create(
        name, upstream, token,
        disable_metrics, streaming_mode, debug_mode,
        refresh_interval_milliseconds, metrics_interval_milliseconds
    );

    if (error != NULL) {
        zend_throw_exception(NULL, error, 0);
        free(error);
        RETURN_THROWS();
    }

    object_init_ex(return_value, unleash_ce);

    // Unwrap the object to get our C struct
    unleash_client_object *intern = unleash_client_from_obj(Z_OBJ_P(return_value));

    // Store the ID directly in C memory (Fast!)
    // We duplicate it because 'name' argument memory belongs to the Zend Engine stack
    intern->client_name = zend_string_copy(name);
}

PHP_METHOD(Iandenh_Unleash_Client, isEnabled) {
    zend_string *feature_name = NULL;
    zend_array *context = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_STR(feature_name)
        Z_PARAM_OPTIONAL
        Z_PARAM_ARRAY_HT(context)
    ZEND_PARSE_PARAMETERS_END();

    unleash_client_object *intern = unleash_client_from_obj(Z_OBJ_P(ZEND_THIS));
    if (!intern->client_name) {
        zend_throw_exception(NULL, "Client not initialized", 0);
        RETURN_THROWS();
    }

    struct go_client_is_enabled_return ret = go_client_is_enabled(intern->client_name, feature_name, context);

    if (ret.r1 != NULL) {
        zend_throw_exception(NULL, ret.r1, 0);
        free(ret.r1);
        RETURN_THROWS();
    }

    RETURN_BOOL(ret.r0);
}

PHP_METHOD(Iandenh_Unleash_Client, close) {
  ZEND_PARSE_PARAMETERS_NONE();

    unleash_client_object *intern = unleash_client_from_obj(Z_OBJ_P(ZEND_THIS));
    if (!intern->client_name) {
        return;
    }

    char *error = go_client_close(intern->client_name);

    if (error != NULL) {
        zend_throw_exception(NULL, error, 0);
        free(error);
        RETURN_THROWS();
    }
}

zend_module_entry unleash_module_entry = {
    STANDARD_MODULE_HEADER,
    "frankenphp-unleash",
    NULL, /* Functions */
    PHP_MINIT(unleash), /* MINIT */
    NULL, /* MSHUTDOWN */
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    "0.1.0", /* Version */
    STANDARD_MODULE_PROPERTIES
};