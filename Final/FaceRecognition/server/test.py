from flask import Flask, request, jsonify
import os

app = Flask(__name__)

UPLOAD_FOLDER = 'uploads'
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route('/upload', methods=['POST'])
def upload():
    # Verificar si se recibió un archivo
    if 'image' not in request.files:
        return jsonify({'error': 'No se recibió ninguna imagen'}), 400
    
    # Obtener el archivo de la solicitud
    image_file = request.files['image']
    
    # Guardar la imagen en el servidor
    if image_file:
        filename = os.path.join(app.config['UPLOAD_FOLDER'], image_file.filename)
        image_file.save(filename)
        return jsonify({'message': 'Imagen recibida y guardada correctamente', 'filename': filename}), 200
    
    return jsonify({'error': 'Error al guardar la imagen'}), 500

if __name__ == '__main__':
    app.run(debug=True)
