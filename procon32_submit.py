import requests
#予行演習用サーバ
URL="https://procon32-pre.kosen.work"
#URL = "https://procon32-practice.kosen.work"

#適当に設定したパスなのであまり気にしないでください
File_Path="C:/Procon32_Simulator/puzzle_solver/puzzle_text/puzzle_solution.txt"

#--token=b604db66dd97b24ee2a6895034893add998a995a2fd3f52fb0ae1a213dee8fa1
def submit(url,file_path):
    token="b604db66dd97b24ee2a6895034893add998a995a2fd3f52fb0ae1a213dee8fa1"
    endpoint = url
    with open(file_path,'r') as f:
        fdata=f.read()
        r = requests.post(endpoint, headers={"procon-token": token}, data=fdata)
        #r = requests.post(endpoint, headers={"procon-token": token}, data=f)
        if r.status_code == 200:
            print("提出完了")
            return
        #r.status_code==400 format error 提出できていない
        print({r.status_code},{r.text.strip()})
if __name__ == "__main__":
    submit(URL,File_Path)