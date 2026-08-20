<?php
require_once __DIR__ . '/type.php';

function trip(): array {
    return [
        new Triplet('/version', '{}', '{"once":true}'),
        new Triplet('/storage', '{}', '{"once":true}'),
        new Triplet('sql.help', '{}', '{"once":true}')
    ];
}
