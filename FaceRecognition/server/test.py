from flask import Flask, request, jsonify
import base64
import os
import time

app = Flask(__name__)

@app.route('/guardar-imagen', methods=['POST'])
def guardar_imagen():
    try:
        imagen_codificada = request.json['imagen']
        
        imagen_decodificada = base64.b64decode(imagen_codificada)

        directorio_imagenes = './imagenes'
        if not os.path.exists(directorio_imagenes):
            os.makedirs(directorio_imagenes)
        
        nombre_archivo = f"imagen_{int(time.time())}.png"
        ruta_imagen = os.path.join(directorio_imagenes, nombre_archivo)
        with open(ruta_imagen, 'wb') as f:
            f.write(imagen_decodificada)
        
        print(f"Imagen guardada en: {ruta_imagen}")
        return jsonify({'mensaje': 'Imagen guardada correctamente'}), 200
    except Exception as e:
        print(f"Error al guardar la imagen: {e}")
        return jsonify({'error': 'Error al guardar la imagen'}), 500

if __name__ == '__main__':
    app.run(debug=True)
