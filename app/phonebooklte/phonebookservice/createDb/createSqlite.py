# -*- coding:utf-8 -*-

import sqlite3
import time

db = sqlite3.connect("./contact.db")
cu = db.cursor()


#  create group messages
cu.execute("create table if not exists group1001 (id varchar(50), "
"type int, state int, userid varchar(20), groupid varchar(20), date int8, showmessage varchar(200),isread int, "
"accessorypath varchar(50), content varchar(500), contentname varchar(50), contenttype varchar(20))")
cu.execute("delete from group1001")

ISOTIMEFORMAT="%Y%m%d%H%M%S"
for i in range(1,401):
    val =(i,"1","1","45","1001",time.strftime(ISOTIMEFORMAT,time.localtime()),"","0","",u"我是字符数据"+str(i),"","")
    sql = "insert into group1001 values(?,?,?,?,?,?,?,?,?,?,?,?)"
    cu.execute(sql, val)

#/* tupian */
for i in range(1,401):
    val =(i,"2","1","45","1001",time.strftime(ISOTIMEFORMAT,time.localtime()),"","0","","","messagefiles/image.jpg","jpg")
    sql = "insert into group1001 values(?,?,?,?,?,?,?,?,?,?,?,?)"
    cu.execute(sql, val)

#/* file */
for i in range(1,401):
    val =(i,"3","1","45","1001",time.strftime(ISOTIMEFORMAT,time.localtime()),"","0","","","messagefiles/file.txt","txt")
    sql = "insert into group1001 values(?,?,?,?,?,?,?,?,?,?,?,?)"
    cu.execute(sql, val)

#/* audio */
for i in range(1,401):
    val =(i,"4","1","45","1001",time.strftime(ISOTIMEFORMAT,time.localtime()),"","0","","","messagefiles/audio.mp3","mp3")
    sql = "insert into group1001 values(?,?,?,?,?,?,?,?,?,?,?,?)"
    cu.execute(sql, val)

#/* vedio */
for i in range(1,401):
    val =(i,"5","1","45","1001",time.strftime(ISOTIMEFORMAT,time.localtime()),"","0","","","messagefiles/vedio.mp4","mp4")
    sql = "insert into group1001 values(?,?,?,?,?,?,?,?,?,?,?,?)"
    cu.execute(sql, val)

# create history record
cu.execute("create table if not exists callhistory (id varchar(50) primary key, "
           "dialTarget varchar(200), dialType varchar(20), dialTime int, dialNetworkType varchar(200), "
           "dateStart varchar(50), dateEnd varchar(50), "
           "contactId1 varchar(50), contactId2 varchar(50), contactId3 varchar(50), "
           "contactId4 varchar(50), contactId5 varchar(50), contactId6 varchar(50), "
           "number1 varchar(50), number2 varchar(50), number3 varchar(50), "
           "number4 varchar(50), number5 varchar(50), number6 varchar(50)) ");
cu.execute("delete from callhistory")

for i in range(1,2000):
    if i%3 == 0 :
        dialType = "IN_DIAL";
        dialNetwork = "PS_NETWORK_VIDEO_DIAL";
    elif i%3 == 1 :
        dialType = "OUT_DIAL";
        dialNetwork = "PS_NETWORK_VOICE_DIAL";
    elif i%3 == 2 :
        dialType = "MISS_DIAL";
        dialNetwork = "PS_NETWORK_VOICE_DIAL";
    val =(i,"[\"%d\"]"%(i),dialType,"45",dialNetwork,int(time.strftime(ISOTIMEFORMAT,time.localtime()))+i,int(time.strftime(ISOTIMEFORMAT,time.localtime()))+i+1,
    i,"","","","","",
    18190946318+i, "", "", "", "", "")
    sql = "insert into callhistory values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
    cu.execute(sql, val)
db.commit()
db.close()




