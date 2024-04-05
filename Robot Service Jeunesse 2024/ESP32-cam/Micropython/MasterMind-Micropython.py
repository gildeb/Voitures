import os
import socket
import cv2
import random
import numpy as np
import easyocr
from time import perf_counter

###########################################################################################################
#    Constantes
###########################################################################################################
# Jeu
N = 8                            # chiffres de 1 à 8
nb_tr_max = 12                   # nb max de tours
ncol = 4                         # 4 pions
labels = ('A', 'B', 'C', 'D')    # noms des colonnes
prompt = 'Choisis une position'  # invite
# camera
image_w  = 96   # largeur image camera
image_h  = 96   # hauteur image camera
# Interface
pos_w, pos_h, pad = 70, 50, 10         # dimensions d'une pastille et padding
info_h = 30                            # hauteur de la zone de dialogue
tour_h = pos_h + pad + info_h + pad    # hauteur totale de l'affichage pour 1 tour de jeu
frame_w =  ncol*(pos_w + pad) + pad + 2*image_w # largeur totale de la fenêtre de jeu
frame_h = max(nb_tr_max*tour_h + pad, 2*image_h)# hauteur totale de la fenêtre de jeu
BLEU, VERT, ROUGE, BLANC, NOIR = (255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 255), (0, 0, 0)
font = cv2.FONT_HERSHEY_SIMPLEX
line_type  = cv2.LINE_AA
thickness2 = 2
thickness1 = 1
smallsize  = 0.6    # font size
largesize  = 1.6

###########################################################################################################

class OCR:
    """
    Interface pour la reconnaissance de caractères.
    """
    def __init__(self):
        self.reader  = easyocr.Reader(['fr'])  # Utiliser EasyOCR avec la langue française
        self.width  = image_w
        self.height = image_h
        # self.count = 1
        #  initialisation socket udp
        self.s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.addr_port = ('192.168.4.1', 10086)      # ESP32-CAM address
        self.s.settimeout(1)
        self.color = b'BLACK'
        # self.fd = open('/Users/Gilles/Dropbox/Informatique/Anumby/RSJ 2024/ESP32-cam/mastermind.log','w')
        # self.t = perf_counter()

    def read(self):
        try:
            # self.fd.write('      fin de boucle {:6.3f}\r\n'.format(perf_counter() - self.t))
            # self.fd.write('image {}\r\n'.format(self.count))
            # self.t = perf_counter()
            self.s.sendto(self.color, self.addr_port)
            buf = self.s.recvfrom(50000)
            # self.fd.write('     received {:6.3f}\r\n'.format(perf_counter() - self.t))
            # self.count += 1
            raw_img = np.asarray(bytearray(buf[0]), dtype=np.uint8)
            frame = cv2.imdecode(raw_img, cv2.IMREAD_COLOR)
            # self.fd.write('     decoded {:6.3f}\r\n'.format(perf_counter() - self.t))
            res = self.reader.readtext(frame)
            # self.fd.write('     processed {:6.3f}\r\n'.format(perf_counter() - self.t))
            # print('image ', self.count)
            return res, frame
        except:          # timeout de réception de l'image
            # self.fd.write('     no image {:6.3f}\r\n'.format(perf_counter()-self.t))
            self.s.sendto(self.color, self.addr_port)
            return None, None

###########################################################################################################

class MastermindCV(OCR):
    def __init__(self):

        super().__init__()
        # création de l'affichage
        cv2.namedWindow('MasterMind')
        self.frame = np.zeros((frame_h, frame_w, 3), dtype=np.uint8) # création de l'affichage

        # initialise la combinaison secrète
        self.secret = [random.randint(1,N) for _ in range(ncol)]
        print(self.secret)
        #
        self.no_tr = 0       # on commence au tour 0
        self.nb = 0          # aucun chiffre reconnu pour l'instant
        self.win   = False
        self.init_tour()     # initialisation du 1er tour

    def init_tour(self):
        ''' Initialisation d'un tour de jeu '''
        self.combi = [-1, -1, -1, -1]   # aucune position jouée au depart
        self.info = prompt
        self.display_new_line()
        self.render()

    def render(self):
        ''' Mise à jour de l'affichage à l'écran'''
        cv2.imshow('MasterMind', self.frame)

    def display_new_line(self):
        ''' Affiche une nouvelle ligne de 4 cases vides (en bleu) '''
        y = pad + self.no_tr * tour_h   # position verticale de la nouvelle ligne
        for col in range(ncol):
            x = pad + col * (pos_w + pad)
            cv2.rectangle(self.frame, (x, y), (x + pos_w, y + pos_h), BLEU, -1)
            cv2.putText(self.frame, labels[col], (x + 10, y + pos_h - 5), font,
                        smallsize, BLANC, thickness2, line_type)
            cv2.rectangle(self.frame, (pad, y + pos_h + pad), (ncol*(pos_w + pad), y + pos_h + info_h),
                          BLANC, -1)

        self.display_info()

    def display_info(self):
        ''' Affiche le texte de self.info dans la case d'info '''
        y = pad + self.no_tr * tour_h    # position verticale de la ligne de jeu courante
        cv2.rectangle(self.frame, (pad, y + pos_h + pad), (ncol*(pos_w + pad), y + pos_h + info_h),
                      NOIR, -1)
        cv2.putText(self.frame, self.info, (pad + 10, y + pos_h + info_h -5 ), font,
                    smallsize, (0, 255, 0), thickness1, line_type)

    def display_res(self):
        ''' Affiche le résultat de la reconnaissance de chiffre dans les 4 cases
            non jouées (en bleu) du tour en cours '''
        for k in range(ncol):
            if self.combi[k] == -1:
                self.draw_nb(k)

    def draw_nb(self, pos):
        ''' Affiche dans la case 'pos' du tour de jeu en cours:
            - le chiffre sur fond rouge  si la case a été jouée
            - le chiffre reconnu sur l'image de la caméra sur fond bleu si la case n'a pas été jouée
            - '?' si la case n'a pas été jouée et aucun chiffre n'a été reconnu '''

        x, y = pad + pos * (pos_w + pad), pad + self.no_tr * tour_h
        if self.combi[pos] != -1:
            text, bkgcolor = str(self.combi[pos]), ROUGE
        else:
            text = str(self.nb) if self.nb else '?'
            bkgcolor = BLEU
        cv2.rectangle(self.frame, (x, y), (x + pos_w, y + pos_h), bkgcolor, -1)
        cv2.putText(self.frame, labels[pos], (x + 5, y + pos_h - 5), font,
                    smallsize, BLANC, thickness2, line_type)
        cv2.putText(self.frame, text, (x + 20, y + pos_h - 10), font, largesize, BLANC, thickness2,
                        line_type)

    def display_image(self):
        ''' Affiche l'image caméra '''
        x = pad + ncol*(pad + pos_w)
        # expansion d'un facteur 2 en x et y
        for lig in range(self.height):
            for col in range(self.width):
                self.frame[2*lig:2*lig+2, x+2*col:x+2*col+2] = self.image[lig, col]

    def display_proba(self):
        ''' Affiche le chiffre reconnu et sa probabilité en incrustation sur l'image caméra '''
        x = pad + ncol*(pad + pos_w)
        y = 2*self.height
        if self.nb:
            text = "p({})={:5.2f}".format(str(self.nb), self.max_prob)
            cv2.putText(self.frame, text, (x + 5, y - 7), font, smallsize, BLEU,
                        thickness2, line_type)

    def run(self):
        ''' Boucle du jeu. A chaque passage dans la boucle, on reçoit une image et on lance
            la détection de chiffre. En cas d'entrée clavier, on lance l'action associée '''
        while True:

            self.process_image()

            # détection des touches du clavier
            k = cv2.waitKey(1) & 0xFF

            if k == ord('q'):   # quit
                break
            elif k == ord('x'):
                if self.info == prompt:
                    self.info = '{}-{}-{}-{}'.format(*self.secret)
                else:
                    self.info = prompt
                self.display_info()

            if (not self.win):
                if self.nb:
                    if k == ord('a') or k == ord('A'):
                        self.combi[0] = self.nb
                        self.draw_nb(0)
                    elif k == ord('b') or k == ord('B'):
                        self.combi[1] = self.nb
                        self.draw_nb(1)
                    elif k == ord('c') or k == ord('C'):
                        self.combi[2] = self.nb
                        self.draw_nb(2)
                    elif k == ord('d') or k == ord('D'):
                        self.combi[3] = self.nb
                        self.draw_nb(3)
                    if not (-1 in self.combi):
                        self.info = "Return pour valider"
                        self.display_info()

                if k == 13:            # retour chariot = fin du tour
                    self.result()

        # self.fd.close()
        cv2.destroyAllWindows()

    def process_image(self):
        ''' Lecture de la caméra et lancement de la reconnaissance de chiffre
            L'image et les cases sont mises à jour dans la fenêtre du jeu '''
        def contains_integer(text):
            try:
                return int(text)
            except ValueError:
                return False

        # réception image et reconnaissance du chiffre
        result, self.image = self.read()
        if result == None:   # pas d'image reçue
            return

        # recherche du chiffre de probabilité max
        self.max_prob, self.nb = 0, False
        for (bbox, text, prob) in result:
            if (nb := contains_integer(text)) and 1 <= nb <= 8 and self.max_prob < prob:
                self.max_prob = prob
                self.nb = nb

        # Mise à jour de l'affichage
        self.display_image()
        self.display_res()
        self.display_proba()
        self.render()

        # Mise à jour de la coulur nopixel
        if self.nb:
            if self.max_prob > 0.8:
                self.color = b'GREEN'
                # self.s.sendto(b'GREEN', self.addr_port)
            else:
                self.color = b'BLUE'
                # self.s.sendto(b'BLUE', self.addr_port)
        else:
            self.color = b'RED'
            # self.s.sendto(b'RED', self.addr_port)

    def result(self):
        """ Fin du tour de jeu : analyse la combinaison choisie """
        exact = 0   # nb de chiffres bien placés
        exist = 0   # nb de chiffres mal placés
        off   = 0   # nb de chiffres non présents dans la combinaison secrète
        secret = [self.secret[k] for k in range(ncol)]
        combi  = [self.combi[k] for k in range(ncol)]

        for ind, val in enumerate(self.combi):
            if val == self.secret[ind]:
                secret[ind] = False
                combi[ind]  = False
                exact += 1

        for val in secret:
            if val and (val in combi):
                combi.remove(val)
                exist += 1

        off = ncol - exact - exist

        if exact == ncol:
            self.info = "Bravo! victoire en {} coups".format(self.no_tr + 1)
            self.win = True
        else:
            self.info = "OK={} mal place(s)={}".format(exact, exist)

        self.display_info()
        self.no_tr += 1
        if not self.win: self.init_tour()

###########################################################################################################

def main():
    cv_game = MastermindCV()
    cv_game.run()

if __name__ == "__main__":
    main()
