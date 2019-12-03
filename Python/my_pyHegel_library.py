#!/bin/python
# -*- coding: utf-8 -*-

# # SEE: http://code.activestate.com/recipes/52558/
# class Singleton:
    # """ A python singleton """

    # class __impl:
        # """ Implementation of the singleton interface """

        # def spam(self):
            # """ Test method, return singleton id """
            # return id(self)

    # # storage for the instance reference
    # __instance = None

    # def __init__(self):
        # """ Create singleton instance """
        # # Check whether we already have an instance
        # if Singleton.__instance is None:
            # # Create and remember instance
            # Singleton.__instance = Singleton.__impl()

        # # Store instance reference as the only member in the handle
        # self.__dict__['_Singleton__instance'] = Singleton.__instance

    # def __getattr__(self, attr):
        # """ Delegate access to implementation """
        # return getattr(self.__instance, attr)

    # def __setattr__(self, attr, value):
        # """ Delegate access to implementation """
        # return setattr(self.__instance, attr, value)


# # Test it
# s1 = Singleton()
# print id(s1), s1.spam()

# s2 = Singleton()
# print id(s2), s2.spam()

# # Sample output, the second (inner) id is constant:
# # 8172684 8176268
# # 8168588 8176268



try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    

gz.config(channels=[1], n_S_ch=1024, bits_16=True, gain_dB=0.0, equalizer_en=True, ext_ref='default', _hdr_func=None)
gz.config([1], n_S_ch=gz_samples, gain_dB=gz_gain, bits_16=bits_16, equalizer_en=gz_equalizer, ext_ref=gz_ref)