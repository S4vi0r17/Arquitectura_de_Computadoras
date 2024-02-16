import cv2
import face_recognition

image = cv2.imread("../images/2.png")
face_loc = face_recognition.face_locations(image)[0]
print("face_loc:", face_loc)
face_image_encodings = face_recognition.face_encodings(image, known_face_locations=[face_loc])[0]
print("face_image_encodings:", face_image_encodings)

# URl del streaming
rtsp_url = "http://192.168.1.42:4747/video"

# Inicializar la captura
cap = cv2.VideoCapture(rtsp_url)

# Resolución del video
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

# frame_count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    # frame_count += 1
    # if frame_count % 2 != 0:  # Procesar solo cada segundo fotograma
    #     continue
    
    frame = cv2.flip(frame, 1)

    # "cnn" para mayor precisión
    face_locations = face_recognition.face_locations(frame, model="cnn")
    if face_locations:
        for face_location in face_locations:
            face_frame_encodings = face_recognition.face_encodings(frame, known_face_locations=[face_location])[0]
            result = face_recognition.compare_faces([face_image_encodings], face_frame_encodings)
            print("Result:", result)

            if result[0]:
                text = "Escribas"
                color = (125, 220, 0)
            else:
                text = "Desconocido"
                color = (50, 50, 255)

            cv2.rectangle(frame, (face_location[3], face_location[0]), (face_location[1], face_location[2]), color, 2)
            cv2.putText(frame, text, (face_location[3], face_location[2] + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 1)

    cv2.imshow("Frame", frame)
    k = cv2.waitKey(1)
    if k == 27 & 0xFF:
        break

cap.release()
cv2.destroyAllWindows()
