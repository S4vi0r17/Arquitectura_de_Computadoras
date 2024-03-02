import cv2
import face_recognition

image1 = cv2.imread("../images/5.jpg")
image2 = cv2.imread("../images/ted.jpg")

# Codificaciones faciales
face_encodings1 = face_recognition.face_encodings(image1)[0]
face_encodings2 = face_recognition.face_encodings(image2)[0]

results = face_recognition.compare_faces([face_encodings1], face_encodings2)

# print("results:", results)

if results[0]:
    print("Las imágenes contienen el mismo rostro.")
else:
    print("Las imágenes no contienen el mismo rostro.")