import httplib
import urllib
#import time
import sys

hezeK = sys.argv[1]
tenpK = sys.argv[2]
hezeB = sys.argv[3]
tenpB = sys.argv[4]

conn = httplib.HTTPSConnection("api.thingspeak.com")
conn.connect()

metodoa = 'POST'
uri = '/update'
edukia = {'api_key': 'C67U10JFDF3OYZOD',
          'field1': hezeK,
          'field2': tenpK,
          'field3': hezeB,
          'field4': tenpB}
edukia_coded = urllib.urlencode(edukia)
print edukia_coded
goiburuak = {'Host': "api.thingspeak.com",
             'Content-Type': "application/x-www-form-urlencoded",
             'Content-Length': len(edukia_coded)}
conn.request(metodoa, uri, headers=goiburuak, body=edukia_coded)

erantzuna = conn.getresponse()
estatusa = erantzuna.status
print str(estatusa)

#time.sleep(15)

conn.close()

