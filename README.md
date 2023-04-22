# PowerFactor

Stm32f103c8t6 mikrodenetleyicisi kullanarak yaptığım bir projeden; 

Bu kod parçacığı, gerilim ve akım örneklerini alarak güç faktörünü TRUERMS algoritması ile hesaplamak için kullanılıyor. 

Kodun temel işleyişine göz atalım:

read_samples() fonksiyonu, gerilim ve akım örneklerini alır ve bunları ilgili dizilere kaydeder.
filter_samples() fonksiyonu, örnekler üzerinde düşük geçiren bir filtre uygular. Bu, yüksek frekanslı gürültüyü azaltmaya yardımcı olur.
calculate_true_rms() fonksiyonu, gerilim ve akım örneklerinin gerçek RMS değerlerini hesaplar.
calculate_power_factor() fonksiyonu, gerilim ve akım RMS değerlerini kullanarak güç faktörünü hesaplar.
Son olarak, güç faktörü ekrana yazdırılır.
