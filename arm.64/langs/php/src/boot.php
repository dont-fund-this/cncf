<?php
require_once __DIR__ . '/find.php';
require_once __DIR__ . '/bind.php';

function boot(?string $targetDir = null): array {
    $engines = [];
    if ($envLib = getenv('PAT_LIB')) {
        if ($c = bind($envLib)) {
            $engines[] = $c;
            return $engines;
        }
    }

    $files = find($targetDir);
    foreach ($files as $file) {
        if ($c = bind($file)) {
            $engines[] = $c;
        }
    }
    return $engines;
}
