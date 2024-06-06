#!/bin/bash
# Your script starts here
echo "Content-Type: text/html"
echo ""

# Function to parse query string
parse_query_string() {
    local query_string="$1"
    local IFS='&'
    for param in $query_string; do
        local key="${param%%=*}"
        local value="${param#*=}"
        eval "$key='$value'"
    done
}

# Read the query string from the environment variable
QUERY_STRING=$(echo "$QUERY_STRING" | tr '+' ' ')  # Replace + with space
QUERY_STRING=$(echo -e "${QUERY_STRING//%/\\x}")  # Decode URL-encoded characters

# Parse the query string
parse_query_string "$QUERY_STRING"

# Print the HTML content
echo "<html>"
echo "<head>"
echo "<title>CGI GET Method Handler</title>"
echo "</head>"
echo "<body>"
echo "<h1>CGI GET Method Handler</h1>"

# Display the parsed parameters
echo "<h2>Query Parameters</h2>"
echo "<ul>"

for param in $QUERY_STRING; do
    key="${param%%=*}"
    value="${param#*=}"
    echo "<li><strong>$key</strong>: $value</li>"
done

echo "</ul>"
echo "</body>"
echo "</html>"
