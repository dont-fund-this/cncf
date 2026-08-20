<?php
function find(?string $targetDir = null): array {
    $dir = $targetDir;
    if (!$dir) {
        if (getenv('DIST_DIR')) {
            $dir = getenv('DIST_DIR');
        } else {
            $candidates = [
                __DIR__ . "/../../../dist",
                __DIR__ . "/../../dist",
                "dist",
                "../../dist",
            ];
            $dir = "dist";
            foreach ($candidates as $c) {
                if (is_dir($c)) {
                    $dir = realpath($c);
                    break;
                }
            }
        }
    }
    if (!is_dir($dir)) return [];

    $files = [];
    foreach (scandir($dir) as $item) {
        if ($item === '.' || $item === '..' || $item === '.DS_Store') continue;
        $p = $dir . DIRECTORY_SEPARATOR . $item;
        if (is_file($p)) $files[] = $p;
    }
    return $files;
}
