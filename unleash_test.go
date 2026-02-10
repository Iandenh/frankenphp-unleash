package unleash

import "testing"

func TestToContextMapsStandardFields(t *testing.T) {
	input := map[string]any{
		"userId":        "user-123",
		"sessionId":     "session-456",
		"remoteAddress": "127.0.0.1",
		"environment":   "prod",
		"appName":       "api",
		"currentTime":   "now",
		"custom":        42,
	}

	ctx := toContext(input)

	if ctx.UserId != "user-123" {
		t.Fatalf("expected UserId to be mapped, got %q", ctx.UserId)
	}

	if ctx.SessionId != "session-456" {
		t.Fatalf("expected SessionId to be mapped, got %q", ctx.SessionId)
	}

	if ctx.RemoteAddress != "127.0.0.1" {
		t.Fatalf("expected RemoteAddress to be mapped, got %q", ctx.RemoteAddress)
	}

	if ctx.Environment != "prod" {
		t.Fatalf("expected Environment to be mapped, got %q", ctx.Environment)
	}

	if ctx.AppName != "api" {
		t.Fatalf("expected AppName to be mapped, got %q", ctx.AppName)
	}

	if ctx.CurrentTime != "now" {
		t.Fatalf("expected CurrentTime to be mapped, got %q", ctx.CurrentTime)
	}

	if ctx.Properties == nil {
		t.Fatalf("expected Properties to be initialized for custom values")
	}

	if ctx.Properties["custom"] != "42" {
		t.Fatalf("expected custom property to be stringified, got %q", ctx.Properties["custom"])
	}
}

func TestToContextUsesProvidedProperties(t *testing.T) {
	properties := map[string]string{
		"team": "core",
	}
	input := map[string]any{
		"userId":     "user-1",
		"properties": properties,
		"region":     "eu-west-1",
	}

	ctx := toContext(input)

	if ctx.Properties == nil {
		t.Fatalf("expected Properties to be set from properties input")
	}

	if ctx.Properties["team"] != "core" {
		t.Fatalf("expected properties map to be preserved")
	}

	if ctx.Properties["region"] != "eu-west-1" {
		t.Fatalf("expected extra properties to be merged, got %q", ctx.Properties["region"])
	}
}

func TestToContextAvoidsAllocatingProperties(t *testing.T) {
	input := map[string]any{
		"userId":    "user-1",
		"sessionId": "session-2",
	}

	ctx := toContext(input)

	if ctx.Properties != nil {
		t.Fatalf("expected Properties to remain nil when unused")
	}
}
