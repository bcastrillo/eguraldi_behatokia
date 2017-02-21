import httplib
import urllib

print "\r\n---> estableciendo conexion TCP"
conn = httplib.HTTPSConnection("tic-investigacion-1.appspot.com")
conn.connect()
print "---> conexion TCP establecida"

metodoa = "POST"
uria = "/processForm"
edukia = {'nan': '44343855'}
edukia_coded = urllib.urlencode(edukia)
print edukia_coded
goiburuak = {'Host': "tic-investigacion-1.appspot.com",
             'Content-Type': "application/x-www-form-urlencoded",
             'Content-Length': len(edukia_coded)}
conn.request(metodoa, uria, headers=goiburuak, body=edukia_coded)

erantzuna = conn.getresponse()
estatusa = erantzuna.status
print str(estatusa)
edukia_e = erantzuna.read()
print edukia_e
