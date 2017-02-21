import httplib

print "\r\n---> estableciendo conexion TCP"
conn = httplib.HTTPSConnection("www.google.com")
conn.connect()
print "---> conexion TCP establecida"

metodoa = 'GET'
baliabidea_URI = '/'
eskaera_goiburuak = {'Host': "www.google.com"}
eskaera_edukia = ''
conn.request(method=metodoa, url=baliabidea_URI,
             headers=eskaera_goiburuak, body=eskaera_edukia)
print "---> HTTP eskaera bidali da"

erantzuna = conn.getresponse()
print "---> erantzuna jaso dugu"
print "---> " + str(erantzuna.status)
location = erantzuna.getheader("Location")
print "---> Location: " + location

conn.close()

print("#############################")

print "\r\n---> estableciendo conexion TCP"
conn = httplib.HTTPSConnection("www.google.es")
conn.connect()
print "---> conexion TCP establecida"

metodoa = 'GET'
baliabidea_URI = '/?gfe_rd=cr&ei=kjeGWKTTF_OJ8QeRw4LIDQ'
eskaera_goiburuak = {'Host': "www.google.es"}
eskaera_edukia = ''
conn.request(method=metodoa, url=baliabidea_URI,
             headers=eskaera_goiburuak, body=eskaera_edukia)
print "---> HTTP eskaera bidali da"

erantzuna = conn.getresponse()
print "---> erantzuna jaso dugu"
print "---> " + str(erantzuna.status)
erantzuna_edukia = erantzuna.read()
print erantzuna_edukia

fitxategia = open("google.html", "w")
fitxategia.write(erantzuna_edukia)
fitxategia.close()