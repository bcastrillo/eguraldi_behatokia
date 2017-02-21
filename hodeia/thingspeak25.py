import psutil
import httplib
import urllib

print "\r\n---> estableciendo conexion TCP"
conn = httplib.HTTPSConnection("api.thingspeak.com")
conn.connect()
print "---> conexion TCP establecida"

i = 0
while(i<5):
    cpu = psutil.cpu_percent(interval=15)
    ram = psutil.virtual_memory().percent
    metodoa = 'POST'
    uri = '/update'
    edukia = {'api_key': 'C67U10JFDF3OYZOD',
              'field1': cpu,
              'field2': ram}
    edukia_coded = urllib.urlencode(edukia)
    print edukia_coded
    goiburuak = {'Host': "api.thingspeak.com",
                 'Content-Type': "application/x-www-form-urlencoded",
                 'Content-Length': len(edukia_coded)}
    conn.request(metodoa, uri, headers=goiburuak, body=edukia_coded)

    erantzuna = conn.getresponse()
    estatusa = erantzuna.status
    print str(estatusa)
    i=i+1

metodoa = 'DELETE'
uri = '/channels/219541/feeds'
edukia = {'api_key': 'NIRXWA0YTU70796F'}
edukia_coded = urllib.urlencode(edukia)
print edukia_coded
goiburuak = {'Host': "api.thingspeak.com",
             'Content-Type': "application/x-www-form-urlencoded",
             'Content-Length': len(edukia_coded)}
conn.request(metodoa, uri, headers=goiburuak, body=edukia_coded)

erantzuna = conn.getresponse()
estatusa = erantzuna.status
print str(estatusa)
