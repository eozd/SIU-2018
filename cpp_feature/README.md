# C++ Feature Computer
* Türkçe versiyon aşağıdadır. (see Turkish version below)

This is the C++ library to compute features for a given match.

## Requirements
* g++-5.5.0 and above with full C++14 support
  * clang, MSVC and other compilers are not tested; but the code may work
  without any problems with these compilers as well since the library does 
  not depend on any gcc specific behaviour.
* cmake 3.0 and above
* boost 1.58.0 and above
  * This is the version we have tested; but it may work with older versions such
  as 1.51.0 as well. We use boost only for convex hull computation.
  * boost headers are expected to be found in ```/usr/include/boost``` folder.
* [dkm](https://github.com/genbattle/dkm)
  * Since dkm is header-only it comes shipped with our C++ library under ```include/dkm``` folder.
* [Catch2](https://github.com/catchorg/Catch2)
  * Since Catch2 is header-only it comes shipped with our C++ library under ```include/catch``` folder.

## Building
To build the project, you can run the following commands:
```
chmod +x build.sh
./build.sh release notest
```
This will build the optimized version of the library and produce an executable
called ```feature``` in project root directory.

## Testing
If you want to run the tests to ensure the library works correctly on your
system, type the following commands:
```
./build.sh release test
```
This will build the tests and run them.

## Documentation
If you want to view the doxygen documentation in your browser, first build the
doxygen documentation using
```
./build.sh doc
```
Then, open ```doc/html/index.html``` file with your browser. You can view the
documentation for all functions/classes in all files by viewing the file, or
by simply searching the function/class by its name.

## Running
To see how ```feature``` executable is used, simply type
```
./feature
```

feature takes a raw data path, computes the features and writes a feature csv
file in the given output path.

### Example Usage
To compute features for a file called ```123_rawdata.txt```, run
```
./feature 123_rawdata.txt 123_feature.csv
```

# Computing Features for Several Matches
A convenience script is provided in ```scripts/compute_features_parallel.py```
to compute features for several matches in parallel. To compute features for
all raw match data in a folder **X**, run the following command:
```
mkdir out
python3 scripts/compute_features_parallel.py X
```
This will gather all files with name ```<id>_rawdata.txt``` from directory **X**,
compute features for each dataset and write the results to corresponding files
under ```out```.

### Example Usage
If we have the following directory layout
```
out
scripts
└── compute_features_parallel.py
raw
├── 123_rawdata.txt
└── 456_rawdata.txt
```
we can run
```
python scripts/compute_features_parallel.py raw
```
to obtain the following layout
```
out
├── 123_feature.csv
└── 456_feature.csv
scripts
└── compute_features_parallel.py
raw
├── 123_rawdata.txt
└── 456_rawdata.txt
```

---

# C++ Öznitelik Hesaplayıcı
Bu klasörde bir maçın özniteliklerini hesaplayan C++ kütüphanesi bulunmaktadır.

## Gereksinimler
* g++-5 veya üst versiyonu (C++14 tam olarak desteklenmelidir)
  * clang, MSVC vb. derleyiciler test edilmemiştir; ancak kod g++'a spesifik
  özellikler kullanmadığı için bu derleyicilerle de sorunsuz çalışabilir.
* cmake 3.0 veya üst versiyonu
* boost 1.58.0 veya üst versiyonu
  * 1.58.0 test edilen versiyondur; fakat boost sadece konveks zarf hesaplanırken
  kullanıldığı için 1.51.0 gibi daha eski versiyonlar da kullanılabilir.
  * boost header dosyalarının ```/usr/include/boost``` klasöründe olduğu
  varsayılmaktadır.
* [dkm](https://github.com/genbattle/dkm)
  * dkm sadece header dosyasından oluştuğu için C++ kütüphanemizle beraber
  gelmektedir (```include/dkm``` klasörü).
* [Catch2](https://github.com/catchorg/Catch2)
  * Catch2 sadece header dosyasından oluştuğu için C++ kütüphanemizle beraber
  gelmekdetir (```include/catch``` klasörü).

## Kurulum
Kütüphaneyi kurmak için aşağıdaki komutları giriniz:

```
chmod +x build.sh
./build.sh release notest
```

Bu komutlar kütüphanenin optimize versiyonunu derleyecektir. Derleme sonrası ```feature``` isimli bir uygulama oluşturulacaktır.

## Test Etme
Kütüphanenin doğru çalıştığından emin olmak için testleri derleyip
çalıştırabilirsiniz. Bunun için aşağıdaki komutları giriniz:

```
./build.sh release test
```

## Dokümantasyon
doxygen ile oluşturulmuş dokümantasyonu görmek için öncelikle aşağıdaki komutu
giriniz
```
./build.sh doc
```
Bunun sonrasında ```doc/html/index.html``` dosyasını browserınızla açarak tüm
fonksiyon/class dokümantasyonunu ilgili dosyanın sekmesinde bulabilirsiniz.
Ayrıca fonksiyon/class ismini arama kısmına girerek arama da yapabilirsiniz.

## Çalıştırma
Uygulamanın nasıl çalıştırılacağını öğrenmek için

```
./feature
```

yazın. ```feature``` ham maç datasının ve oluşturulacak öznitelik datasının
yerlerini parametre olarak almaktadır.

### Örnek Kullanım
123 maçının (```123_rawdata.txt```) özniteliklerini hesaplamak için

```
./feature 123_rawdata.txt 123_feature.csv
```
komutunu yazınız. Öznitelikler 123_feature.csv dosyasına kaydedilecektir.

# Birden Fazla Maç İçin Öznitelik Hesaplama
Birden çok maçın özniteliğinin paralel olarak hesaplanması için ```scripts/compute_features_parallel.py```
isminde Python komut dosyası verilmiştir.
**X** klasöründeki tüm ham maç dataları için öznitelik şu şekilde hesaplanabilir:

```
mkdir out
python3 scripts/compute_features_parallel.py X
```

Bu komutlar **X** klasöründeki ve ```<id>_rawdata.txt``` formatındaki her ham maç
datası için öznitelik hesaplayıp öznitelikleri ```out``` klasörüne kaydedecektir

### Örnek Kullanım
Aşağıdaki klasör yapısı varken

```
out
scripts
└── compute_features_parallel.py
raw
├── 123_rawdata.txt
└── 456_rawdata.txt
```

```
python scripts/compute_features_parallel.py raw
```

komutunu girerek öznitelikleri hesaplarız. Bunun sonucunda klasör yapısı
aşağıda verildiği şekle dönüşecektir.

```
out
├── 123_feature.csv
└── 456_feature.csv
scripts
└── compute_features_parallel.py
raw
├── 123_rawdata.txt
└── 456_rawdata.txt
```
