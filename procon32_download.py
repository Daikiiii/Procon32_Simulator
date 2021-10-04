import urllib.parse
import requests
#予行演習用サーバ
URL="https://procon32-pre.kosen.work"
#URL = "https://procon32-practice.kosen.work"
File_Path="C:/Procon32_Simulator/puzzle_solver/puzzle/"

'''
proconサーバから問題を取得します
download(url,File_Path)
url:サーバのurl
File_Path:problem.ppmの保存先
'''
#--token=b604db66dd97b24ee2a6895034893add998a995a2fd3f52fb0ae1a213dee8fa1
def download(url,File_Path):
    #トークン
    token='b604db66dd97b24ee2a6895034893add998a995a2fd3f52fb0ae1a213dee8fa1'
    with open(File_Path+"problem.ppm",'wb') as o:
        endpoint = urllib.parse.urljoin(url, "problem.ppm")
        while True:
            r = requests.get(endpoint, headers={"procon-token": token})
            if r.status_code == 200:
                o.write(r.content)
                print(File_Path,"に","problem.ppmを保存しました",)
                return
            print(r.status_code,r.text.strip())

if __name__ == "__main__":
    #ppmの保存先
    download(URL,File_Path)