from flask import Flask, jsonify
import cv2
import face_recognition

app = Flask(__name__)

@app.route('/analizar_rostros', methods=['GET'])
def analizar_rostros():
    try:
        image = cv2.imread("../images/2.png")
        face_loc = face_recognition.face_locations(image)[0]
        face_image_encodings = face_recognition.face_encodings(image, known_face_locations=[face_loc])[0]

        rtsp_url = "http://192.168.1.43:4747/video"
        cap = cv2.VideoCapture(rtsp_url)

        true_count = 0
        false_count = 0

        i = 0

        while i < 20:
            ret, frame = cap.read()
            if not ret:
                break

            frame = cv2.flip(frame, 1)

            face_locations = face_recognition.face_locations(
                frame, model="hog")
            if face_locations:
                for face_location in face_locations:
                    face_frame_encodings = face_recognition.face_encodings(
                        frame, known_face_locations=[face_location])[0]
                    result = face_recognition.compare_faces(
                        [face_image_encodings], face_frame_encodings)
                    print("Result:", i, result)

                    if result[0]:
                        true_count += 1
                    else:
                        false_count += 1

                    cv2.rectangle(frame, (face_location[3], face_location[0]), (
                        face_location[1], face_location[2]), (255, 255, 255), 2)
            i += 1

        cap.release()

        total_count = true_count + false_count
        if total_count == 0:
            true_percentage = 0
            false_percentage = 0
        else:
            true_percentage = (true_count / total_count) * 100
            false_percentage = (false_count / total_count) * 100

        resultado = true_percentage > false_percentage

        respuesta = {
            "resultado": resultado,
            "true_percentage": true_percentage,
            "false_percentage": false_percentage
        }

        return jsonify(respuesta)

    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True)
