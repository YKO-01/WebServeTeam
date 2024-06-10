#!/bin/bash

# Print HTTP headers
echo "Content-type: text/html"
echo ""

# Parse the query string
QUERY_STRING="$QUERY_STRING" # In CGI scripts, QUERY_STRING contains the GET parameters
IFS='&' read -ra PARAMS <<< "$QUERY_STRING"
declare -A GET_PARAMS
for param in "${PARAMS[@]}"; do
    IFS='=' read -r key value <<< "$param"
    GET_PARAMS["$key"]="$value"
done

# HTML output
echo "<html>"
echo "<head>"
echo "<title>Bash CGI Script - GET Method</title>"
echo "</head>"
echo "<body>"
echo "<h1>GET Parameters</h1>"
echo "<ul>"
for key in "${!GET_PARAMS[@]}"; do
    echo "<li><b>$key:</b> ${GET_PARAMS[$key]}</li>"
done
echo "</ul>"
echo "</body>"
echo "</html>"
