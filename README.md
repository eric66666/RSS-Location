# RSS-Location
實作無線網路LBS中，使用RSS訊號地圖的定位方法<br>
實作1NN、3NN、5NN比對方法，計算定位錯誤率<br>
此專案使用Building 2 Floor 3的資料做訓練與測試<br>
1. 採用Euclidean distance作為distance計算之演算法，撰寫對應之程式。某筆資料中未偵測到的AP訊號可定為100或是-104。<br>
2. 一筆資料做一次測試，將新的fingerprint與radio database的每項計算distance，以距離最近的fingerprint其代表的位置作為預估的位置。計算真實座標與預估座標的誤差。最後算出ValidationData中的各自距離誤差與平均值。<br>
3. 採用KNN (K = 3 或 5)的方法，挑選最近K筆資料將其K個座標值平均作為預估位置。同樣計算ValidationData中距離誤差與平均值。<br>

[方法參考網址](http://indoorlocplatform.uji.es/methods/knn/)<br> 
[結果參考網址](http://indoorlocplatform.uji.es/dashboard/ujiindoorloc/)<br>







有任何問題請寄信與我聯繫：ericchu13579@gmail.com
