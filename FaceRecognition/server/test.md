# Api para envio:
http://127.0.0.1:4000/datos [POST]
```json
{
    "temperatura": 28,
    "humedad": 55,
    "ventiladorEncendido": true
}
```
# Api para recibir:
http://127.0.0.1:4000/api/datos [GET]
# Api para enviar imagen:
http://127.0.0.1:5000/upload

Body: form-data
Key: file
Value: imagen.jpg