<?php

/** @generate-class-entries */

namespace Iandenh\Unleash {
    final class Client
    {
        public static function getOrCreate(
            string $name,
            string $upstream,
            string $token,
            bool $disableMetrics = false,
            bool $streamingMode = false,
            bool $debugMode = false,
            int $refreshIntervalMilliseconds = 15000,
            int $metricsIntervalMilliseconds = 60000,
        ): \Iandenh\Unleash\Client {
        }

        public function isEnabled(string $featureFlag, array $context = []): bool
        {
        }

        /**
         * Closes the client and destroys it globally.
         */
        public function close(): void
        {
        }
    }
}
