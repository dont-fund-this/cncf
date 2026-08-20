<?php
require_once __DIR__ . '/boot.php';
require_once __DIR__ . '/trip.php';

$targetDir = $argv[1] ?? null;
$dist = boot($targetDir);

if (count($dist) > 0) {
    foreach ($dist as $d) {
        foreach (trip() as $t) {
            $d->ffi->Pump($t->address, $t->payload, $t->options);
        }
    }
}

$out = [
    'lang' => 'php',
    'status' => 'ready',
    'engines' => count($dist)
];

echo json_encode($out, JSON_PRETTY_PRINT) . PHP_EOL;
