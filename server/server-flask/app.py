from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/datos', methods=['POST'])
def recibir_datos():
    datos = request.json
    temperatura = datos.get('temperatura')
    humedad = datos.get('humedad')

    if temperatura is not None and humedad is not None:
        if temperatura > 30 or humedad > 70:
            ventilador_encendido = True
        else:
            ventilador_encendido = False
    else:
        ventilador_encendido = False

    return jsonify({'ventiladorEncendido': ventilador_encendido})


if __name__ == '__main__':
    app.run(debug=True)
