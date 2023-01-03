import face_recognition
import cv2
import numpy as np
import os
from numpy import array
import mysql.connector as mysql

HOST = "192.168.8.101"
DATABASE = "medtest03"
USER = "root"
PASSWORD = "123456"

mydb = mysql.connect(host=HOST, 
                              database=DATABASE, 
                              user=USER, 
                              password=PASSWORD)
print("Connected to:", mydb.get_server_info())
cursor = mydb.cursor()
cursor.execute("SELECT first_name, face FROM medtest03.medapp_elder;")

#----------------------------------- face detect & crop then compare ---------------------------------------------------

# folder picture for train 
know_encodings = []
know_name = []
# path = "D:/project_code/all_code_show/sort_face_pic"
# for image in os.listdir(path): # from main folder --> Get each person's filename
#     all_img = face_recognition.load_image_file(path+"/"+image)  
#     all_encodeing = face_recognition.face_encodings(all_img)[0]

#     know_encodings.append(all_encodeing)
#     name_face = str(image).replace('.jpg','')
    # know_name.append(name_face)

# k = "[-0.07472766190767288, 0.03625894710421562, 0.052897192537784576, -0.008510967716574669, -0.0879981741309166, -0.06744565814733505, -0.011713775806128979, -0.15471844375133514, 0.1056465208530426, -0.1402784138917923, 0.22443053126335144, -0.05385174602270126, -0.19615556299686432, -0.10353958606719971, -0.027266118675470352, 0.19365541636943817, -0.14834672212600708, -0.1156153678894043, -0.08929690718650818, -0.04244399815797806, 0.04688692465424538, -0.01865711808204651, 0.015020297840237617, 0.037078529596328735, -0.055544883012771606, -0.35575607419013977, -0.061176758259534836, -0.036906104534864426, -0.022603213787078857, -0.04061324894428253, -0.07095634937286377, 0.11230866611003876, -0.1875348538160324, -0.04921821504831314, 0.00421843770891428, 0.13224442303180695, -0.04055427759885788, -0.10348492115736008, 0.18955285847187042, -0.05357304960489273, -0.22745124995708466, -0.03240404278039932, 0.09037165343761444, 0.19422690570354462, 0.1719336360692978, 0.014621959067881107, 0.026857955381274223, -0.13171596825122833, 0.1420573592185974, -0.15386298298835754, 0.009443447925150394, 0.12437509000301361, 0.042717404663562775, 0.04714943841099739, -0.02072872593998909, -0.1280132234096527, 0.03680313378572464, 0.08447660505771637, -0.10271589457988739, -0.047998543828725815, 0.09336800128221512, -0.12476881593465805, -0.07054615765810013, -0.09533178061246872, 0.22730578482151031, 0.08165405690670013, -0.08743041008710861, -0.14754855632781982, 0.1275302767753601, -0.1796378195285797, -0.09362443536520004, 0.0028128623962402344, -0.1440218836069107, -0.17705339193344116, -0.3243306577205658, -0.0022917669266462326, 0.3409010171890259, 0.10038504749536514, -0.16458012163639069, 0.049380961805582047, -0.04108920693397522, -0.003373668994754553, 0.18321862816810608, 0.1748560220003128, -0.007422377355396748, 0.05828871205449104, -0.11814035475254059, 0.008271176367998123, 0.24949431419372559, -0.05237792059779167, -0.025985000655055046, 0.21375319361686707, 0.008632730692625046, 0.05472147837281227, 0.026662643998861313, -0.0031892703846096992, -0.04799695312976837, 0.05397171154618263, -0.17343395948410034, 0.005502501502633095, 0.09204281866550446, -0.03615974262356758, -0.00846132542937994, 0.10369076579809189, -0.14936211705207825, 0.08084024488925934, -0.017685450613498688, 0.07350143790245056, 0.08913614600896835, -0.07157249003648758, -0.13553868234157562, -0.1114080399274826, 0.11512506008148193, -0.16059768199920654, 0.13134458661079407, 0.16479496657848358, 0.058394789695739746, 0.12656021118164062, 0.16002711653709412, 0.12825755774974823, -0.00781743973493576, -0.04432757943868637, -0.2630636990070343, 0.01576661504805088, 0.14391887187957764, 0.007117823697626591, 0.0699545219540596, 0.011936144903302193]"

for i in cursor:
    carry = i[1].replace('"', '')
    pic = eval('np.array(' + carry + ')')
    know_encodings.append(pic)
    know_name.append(i[0])

cam = cv2.VideoCapture(0)    # for video capturing from cameras (camera_index)
cam.set(cv2.CAP_PROP_FRAME_WIDTH, 640)  # set frame width
cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)  # set frame height


process_this_frame = True
while(True):
    # capture image
    ret, frame = cam.read() # capture img frame-by-frame
    frame = cv2.flip(frame, 1)  # flip camera in the y-axis (mirror)

    if process_this_frame :
        # Convert the image to small and from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
        small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)
        rgb_small_frame = cv2.cvtColor(small_frame, cv2.COLOR_BGR2RGB)
        
        # Find all faces and do face encodings in camera
        face_locations = face_recognition.face_locations(rgb_small_frame)
        face_encoding = face_recognition.face_encodings(rgb_small_frame, face_locations)

    face_names = []
    face_percents = []
    for encoding in face_encoding :
        face_distance = face_recognition.face_distance(know_encodings,encoding)
        best_match_index = np.argmin(face_distance)
        face_percent = 1-face_distance[best_match_index]
        
        # if similar > 60 % return name else = unknow
        if face_percent > 0.6 :
            name = know_name[best_match_index]
            percent = round(face_percent*100,2)
        else:
            name = "Unknow"
            percent = 0
        face_names.append(name)
        face_percents.append(percent)
    process_this_frame = not process_this_frame

    # Draw a rectangle around the faces if detect
    for (top, right, bottom, left), name, percent in zip(face_locations, face_names, face_percents):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        top *= 4
        right *= 4
        bottom *= 4
        left *= 4
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)
        font = cv2.FONT_HERSHEY_DUPLEX
        # cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)
        cv2.putText(frame, str(percent), (left + 6, bottom+23), font, 1.0, (255, 255, 255), 1)

    # show image and rec frame that detect face
    cv2.imshow("frame", frame)

    # wait for key event
    command = cv2.waitKey(10)
    # if press 'q' -> exit program
    if command == ord('q'):
        print ("Ending program")
        break  # end program
 