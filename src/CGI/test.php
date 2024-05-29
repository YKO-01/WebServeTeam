#!/usr/bin/env php
<?php
// Retrieve the value of the environment variable QUERY_STRING
$queryString = $_SERVER["PATH_INFO"];

// Output the value of the QUERY_STRING environment variable
echo "REQUEST_METHOD: " . $queryString ." \n";
echo "name: " . $_SERVER["QUERY_STRING"] ." \n";
?>
