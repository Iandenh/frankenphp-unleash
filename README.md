# FrankenPHP Unleash Extension

This implements a fast unleash extension that is using the Unleash go client in the background.

# Installation:

```bash
CGO_ENABLED=1 \
CGO_CFLAGS="$(php-config --includes) -D_GNU_SOURCE" \
CGO_LDFLAGS="$(php-config --ldflags) $(php-config --libs)" \
xcaddy build \
    --output frankenphp \
    --with github.com/dunglas/frankenphp/caddy \
    --with github.com/dunglas/caddy-cbrotli \
    --with github.com/dunglas/mercure/caddy \
    --with github.com/dunglas/vulcain/caddy \
    --with github.com/iandenh/frankenphp-unleash
```

# Usage

```php
use Iandenh\Unleash\Client;

// Creating a global unleash client named 'api'. Client will be reused.
$client = Client::getOrCreate("api", "https://unleash.example.com/api", "", streamingMode: true);

// Get enabled status. Second argument is the context.
$client->isEnabled('feature_name', ['userId' => 123]);

// If needed the client can be closed (next time a new connection will be created)
$client->close();
```