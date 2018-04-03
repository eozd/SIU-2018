# Notebooks
* Türkçe versiyon aşağıdadır. (see Turkish version below)

In this file, we describe the purpose of each file in notebooks folder.

## Core Notebooks
These are the notebooks used during model development and optimization.

---

### requirements.ipynb
This is the notebook you can use to check if your system meets the requirements
to run all the notebooks.

---

### feature\_analysis.ipynb
In this notebook, we analyse player coordinate data by plotting various simple
features such as average, maximum and minimum coordinates, inner distances,
referee position, etc. We try to find relations between outliers or patterns
in these features and the events we wish to predict. Additionally, we postulate
ideas about how each related feature family can be effective in predicting
certain events.

This was our first notebook when we started this project. Therefore, several
ideas written in this document may be replaced or not used at all. However,
keeping this notebook as a reference may prove useful in further studies.

---

### try\_clustering\_features.ipynb
In this notebook, we use k-means clustering algorithm and compute various
derivative features. Then, we check if these new features can be used in our
final model by visualizing the obtained clusters.

---

### feature\_construction.ipynb
This is the notebook in which we compute features from player coordinate data.
This notebook contains our initial feature construction code. However, feature
computation implementation in this notebook is not particularly fast. It may
take minutes to compute features for a single match which is completely
undesirable. Therefore, later we have written the feature computation part of
the code in C++ which runs significantly faster (a few seconds to compute
features for a single match). You can read more about C++ feature computation
code under ```cpp_feature``` directory.

---

### feature\_selection.ipynb
In this notebook, we use depth-limited decision trees to see which features are
important when dividing the dataset according to gini impurity measure. We
draw confusion matrices, feature importance graphs and the decision trees
themselves for visualization.

---

### feature\_selection\_sampling.ipynb
In this notebook, we try oversampling algorithms to increase the number of
penalty samples in our training samples and check if this helps the classifier
in any way. Again, we plot confusion matrices, feature importance graphs, and
the decision trees themselves for visualization.

---

### dataset\_construction.ipynb
In this notebook, we construct training and test datasets using already computed
feature datasets.

---

### model\_optimization.ipynb
In this notebook, we use grid search and several different random forest
parameter configurations to find the random forest model with highest prediction
power.

---

### classifier\_testing.ipynb
In this notebook, we test random forest classifiers and our final optimal
model. Untrained random forests are tested using cross validation on the
training set. Our final optimized and trained model is tested using held-out
test set. In both cases, we plot normalized confusion matrices and
feature importance graphs.

---

## Miscellaneous Notebooks
These are the helper notebooks we have used during the project that may not be
directly related with current model development process

---

### match\_visualization.ipynb
This is the notebook to create 2D match visualizations from player coordinate
data using matplotlib. Both second-apart animations and snapshots can be
created and exported using vector image formats such as svg or pdf.

---

### hasball\_from\_raw.ipynb
This is the notebook we have used to create hasball datasets (which team has
the ball), from labelled data. This notebook is not used anymore; but it is kept
for reference purposes.

---

### pickle\_to\_csv.ipynb
This notebook was used to convert pickle formatted match data into csv files.
Although it is not used anymore, we keep it for reference purposes.

---

### model\_to\_pmml.ipynb
At one point in the project, we needed to convert our scikit-learn models to
PMML format. This notebook is not used anymore; however, it is kept for
reference purposes.

---

## utils.py
This file contains the common utility functions that are used from different
notebooks.

===========================================
===========================================

# Notlar
Bu dosyada notebooks klasöründeki her dosyanın amacı anlatılacaktır.

## Esas Notlar
Esas notlarda model geliştirme ve optimizasyonu anlatılmaktadır.

---

### requirements.ipynb
Bu notebook ile sisteminizde gerekli kütüphanelerin yüklü olup olmadığını
anlayabilirsiniz.

---

### feature\_analysis.ipynb
Bu notebook ile oyuncu koordinat verisinden elde edilen ortalama, maksimum ve
minimum koordinat, içsel uzaklık, hakem pozisyonu, vb. öznitelikler
incelenmiştir. Bu özniteliklerin uç değerleri ve örüntüleri ile tahmin
edilmeye çalışılan olaylar arasında bağlantılar bulunması hedeflenmiştir.
Bununla beraber, benzer özniteliklerden oluşan öznitelik ailelerinin, olayları
tahmin etmede nasıl etkili olabileceği hakkında fikirler yürütülmüştür.

Bu notebook projeye başladığımızda ortaya koyduğumuz fikirlerden meydana
gelmektedir. Dolayısıyla yazılı fikirlerin bazıları sonraki aşamalarda
değiştirilmiş ya da kaldırılmıştır. Yine de bu fikirlerin referans olarak
saklanması ve gösterilmesi ileriki çalışmalarda yararlı olabilir.

---

### try\_clustering\_features.ipynb
Bu notebook ile k-ortalamalar yöntemi kullanılarak çeşitli öznitelikler
türetilmiştir. Daha sonra bu yeni özniteliklerin nihai modelde kullanılıp
kullanılamayacağını test etmek amacıyla görseller çizilmiştir.

---

### feature\_construction.ipynb
Bu notebook ile oyuncu koordinat verisinden öznitelikler hesaplanmıştır.
Buradaki kodlar öznitelik hesaplama amacıyla yazdığımız ilk kodlardır. Ancak
kodlar yeterince hızlı değildir. Sadece bir maçın özniteliklerini hesaplamak
dakikalar sürebilmektedir. Bu sebeple projenin ilerleyen kısımlarında
öznitelik hesaplama kısmı C++ dilinde yazılmıştır. C++ kodu çok daha hızlı
çalışmaktadır (bütün bir maçın özniteliklerini hesaplamak birkaç saniye
almaktadır.). C++ öznitelik hesaplama kodu hakkında daha fazla bilgiyi
```cpp_feature``` klasöründe bulabilirsiniz.

---

### feature\_selection.ipynb
Bu notebook ile derinliği kısıtlanmış karar ağaçları kullanılarak, hangi
özniteliklerin veri setini Gini katışıklığına (gini impurity) göre bölerken
en önemli olduğu bulunmuştur. Bu amaçla hata matrisleri, öznitelik önem
grafikleri ve karar ağaçlarının kendileri çizdirilmiştir.

---

### feature\_selection\_sampling.ipynb
Bu notebook ile aşırı örneklendirme (oversampling) algoritmaları ile penaltı
olaylarının sayısı artırılmış, ve bunun sonucunda sınıflandırıcının
performansında artışlar olup olmadığı test edilmiştir. Tekrardan, bu amaçla
hata matrisleri, öznitelik önem grafikleri ve karar ağaçlarının kendileri
çizdirilmiştir.

---

### dataset\_construction.ipynb
Bu notebook ile önceden hesaplanmış öznitelikler verileri kullanılarak
eğitim ve test kümeleri oluşturulmuştur.

---

### model\_optimization.ipynb
Bu notebook ile çeşitli parametre kombinasyonlarının eksiksiz biçimde
denendiği hiperparametre optimizasyonu yapılarak en yüksek tahmin etme gücüne
sahip sınıflandırıcı bulunmuştur.

---

### classifier\_testing.ipynb
Bu notebook ile rastgele orman sınıflandırıcıları ve nihai optilam model test
edilmiştir. Daha önce eğitilmemiş rastgele ormanlar çapraz geçerlilik sınaması
(cross validation) ile test edilirken önceden eğitilmiş nihai optimal model
görmediği test verisi üzerinde sınanmıştır. Her iki durumda da normalize
edilmiş hata matrisleri ve öznitelik önem grafikleri çizdirilmiştir.

---

## Miscellaneous Notebooks
Bu kısımda model geliştirme ile direkt olarak ilgili olmayan fakat
proje boyunca kullandığımız yardımcı notebook'ları anlatacağız.

---

### match\_visualization.ipynb
Bu notebook ile oyuncu koordinat verisi ve matplotlib kütüphanesi kullanılarak
maçlar 2 boyutta görselleştirilmiştir. Hem saniye bazlı animasyonlar hem de
anlık durumlar oluşturulup svg ya da pdf gibi vektör formatlarda kaydedilebilir.

---

### hasball\_from\_raw.ipynb
Bu notebook ile etiketlenmiş veriden hasball verisi (hangi takımın topa sahip
olduğu verisi) oluşturulmuştur. Artık kullanılmamasına rağmen referans olarak
bırakılmıştır.

---

### pickle\_to\_csv.ipynb
Bu notebook ile pickle formatındaki maç verisi csv formatına dönüştürülmüştür.
Notebook artık kullanılmamasına rağmen referans olarak bırakılmıştır.

---

### model\_to\_pmml.ipynb
Projenin bir noktasında scikit-learn kütüphanesi ile geliştirdiğimiz modeli
PMML formatına aktarmamız gerekmiştir. Bu notebook artık kullanılmamasına
rağmen referans olarak bırakılmıştır.

---

## utils.py
Bu dosya çeşitli notebook kodları tarafından kullanılan ortak fonksiyonları
içermektedir.
