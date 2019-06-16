import mido
import rtmidi
import time

play = False
conv = dict()

port = mido.open_output(name='IAC Driver Bus 1')

def makeDict():
    f = open("conv.csv",'r')
    for line in f:
        x = line.strip().split(",")
        x[3] = x[3].split("/")[0].lower()
        if x[3][1]=='#': x[3] = x[3][0]+"s"+x[3][2]
        conv[x[0]] = [x[3],x[5]]
    f.close()

def playTrack(track):
    
    notes = []
    for msg in track: 
        if msg.type=="note_on":
            print(msg)
            s = str(msg)
            note = {x.split("=")[0]:x.split("=")[1] for x in s.split(" ")[1:]}
            if note["velocity"]!="0": note["note"] = ["0","0"]
            else: note["note"] = conv[note["note"]]
            note["time"] = int(note["time"])//113
            if note["time"]:
                notes.append("\t\t{"+note["note"][0]+","+str(note["time"])+"},")
            if play:
                if note["time"]: 
                    print(note)
                time.sleep(msg.time/2048)
                port.send(msg)
    return notes∂d     

def playSong(song):
    mid = mido.MidiFile(song["file"])
    for msg in mid.play():
        port.send(msg)
        s = str(msg)
        if not msg.is_meta and s.startswith("note_on"):
            note = {x.split("=")[0]:x.split("=")[1] for x in s.split(" ")[1:]}
            note["note"] = conv[note["note"]]
            print(note)

songs = [{
    "title": "Faded - A.Walker",
    "file": 'Alan_Walker_-_Faded.mid',
    "varName": "faded",
    "beat": "227"
}]

def main():        
    for song in songs: 
        mid = mido.MidiFile(song["file"])
        for i in range(len(mid.tracks)):
            notes = playTrack(mid.tracks[i]) 
            f = open(song["varName"]+"_"+str(i)+".txt",'w')
            f.write("Song "+song["varName"]+"={\n") 
            f.write("\t"+str(len(notes))+",\""+song["title"]+"\",{\n") 
            for note in notes: f.write(note+"\n") 
            f.write("}") 
            f.write("};") 
            f.close()    
    
        
makeDict()
playTrack(mido.MidiFile(songs[0]["file"]).tracks[1])