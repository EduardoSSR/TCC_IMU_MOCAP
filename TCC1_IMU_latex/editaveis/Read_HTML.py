from urllib.request import urlopen
from bs4 import BeautifulSoup
import timeit

try:
    inicio = timeit.default_timer()
    cont = 0
    while(True):
        cont+=1
        url = "http://192.168.4.1/D"
        html = urlopen(url).read()
        soup = BeautifulSoup(html)

        # get text
        text = soup.get_text()
        
        print('Resultado: ',text, '\n')
        
except KeyboardInterrupt:
        fim = timeit.default_timer()
        tempo = fim - inicio
        
        print('Tempo medio de', cont ,'execucoes foi de ',tempo/cont,' s', '\n')
        print('O tempo total foi:',tempo)
        
        print('Laco interrompido.\n')
