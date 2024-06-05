DATA=$(base64 "big_file.iso")
echo "{ \"data\": \"$DATA\" }" | curl -X POST -H "Content-Type: application/json" -d @- https://localhost:6969
