import sys, os
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/Histograms-OTF/')
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/aCorrs-OTF//')
from acorrs_otf import ACorrUpTo, set_num_threads
from histograms_otf import hist1dNbits, hist2dNbits

test = False

make_dir("C://Projets/TimeDomain_Guzik/Data/%D/Tests_Initiaux/" + ("test" if test else ""))

# VARIABLES
gz_samples = 32e9 if not test else 1e6
gz_gain = 15
num_acorrs = 1024 if not test else 5
hist_bits = 16

min_bias = -30
max_bias = 30
num_bias = 61 if not test else 2
updown = False

# Loading devices
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        gz = instruments.guzik_adp7104()
except:
    gz = instruments.guzik_adp7104()
gz.config([1], n_S_ch=gz_samples, gain_dB=gz_gain, bits_16=True)
load("yo10")
set(yo10,0)
set(yo10.output_en, True)

# APPAREIL VIRTUEL
# gz_hac: Yields the histogram of the signal as well as its autocorrelation and a snippet
def gz_snip_wrap():
    get(gz.fetch);
    snip = gz.fetch.getcache()[:10001]
    return snip
def gz_h_wrap():
    v = gz.fetch.getcache()
    h = hist1dNbits(v, 16)
    del v
    return h
def gz_ac_wrap():
    v = gz.fetch.getcache()
    a = ACorrUpTo(num_acorrs, v)
    del v
    return a.res

## The actual virtual instruments
gz_snip = instruments.FunctionWrap(getfunc=gz_snip_wrap, basedev=gz.fetch, multi=('Snippet',))
gz_h = instruments.FunctionWrap(getfunc=gz_h_wrap, basedev=gz.fetch, multi=('Histogram',))
gz_ac = instruments.FunctionWrap(getfunc=gz_ac_wrap, basedev=gz.fetch, multi=('ACorrUpTo{}'.format(num_acorrs),))

                      
# The EXPERIMENT
outDevice = [(gz_snip, dict(bin='.npz')), (gz_h, dict(bin='.npz')), (gz_ac, dict(bin='.npz'))]

set(yo10,0)
set(yo10.range,30)

sweep(yo10, min_bias, max_bias, num_bias, out=outDevice, beforewait=5, updown=updown, filename='Test_initial_%i.txt')

# POST EXPERIMENT 

set(yo10,0)