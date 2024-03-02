from flask import Flask, request, jsonify
import base64
import cv2
import numpy as np
import io  
import face_recognition

app = Flask(__name__)

ultimosDatos = {
    "temperatura": 0,
    "humedad": 0,
    "ventiladorEncendido": False,
}

@app.route('/datos', methods=['POST'])
def recibir_datos():
    datos = request.json
    temperatura = datos.get('temperatura')
    humedad = datos.get('humedad')
    ventilador_encendido = datos.get('ventiladorEncendido')
    
    print(f"Temperatura: {temperatura}°C, Humedad: {humedad}%, Ventilador: {'Encendido' if ventilador_encendido else 'Apagado'}")
    
    global ultimosDatos
    ultimosDatos = {
        "temperatura": temperatura,
        "humedad": humedad,
        "ventiladorEncendido": ventilador_encendido,
    }
    
    return 'Datos recibidos correctamente', 200

@app.route('/api/datos', methods=['GET'])
def obtener_ultimos_datos():
    return jsonify(ultimosDatos), 200


if __name__ == '__main__':
    app.run(debug=True, port=4000)