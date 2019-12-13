"""VERIFIER LE NOM DES INSTRUMENTS : tc3, dmm13, yo8 au besoin. Debut des mesures 29 juillet 2016"""
execfile("C:/Projets/Jonction_TBias/Code/sweepshift.py")

temp_cryo=get(tc3.fetch)

Temperatures=[0.05+0.05*i for i in range(16)]
sweep_S2V = True
        

while(1):

    set(moy_dif.moydev,0)
    for Temp in Temperatures:
        if Temp<=0.02:
                set(tc3.heater_range,0.001)	
        if Temp>0.02 and Temp<=0.15:
                set(tc3.heater_range,0.00316)
        elif Temp>0.15:
                set(tc3.heater_range,0.01)

        if Temp<0.010:
                Filename = "_00{0}mK_%T_.txt".format(int(round(Temp*1000,-1)))
        elif Temp<0.100 and Temp>=0.010:
                Filename = "_0{0}mK_%T_.txt".format(int(round(Temp*1000,-1)))
        elif Temp>=0.100:
                Filename = "_{0}mK_%T_.txt".format(int(round(Temp*1000,-1)))

        Filenam = "calibration_up" + Filename
        set(tc3.sp,Temp)

        # Get to temperature.
        temp_cryo=get(tc3.fetch) 

        while(temp_cryo[6]<Temp):
                wait(10)
                temp_cryo=get(tc3.fetch)

        # Wait for stabilisation. 
        wait(1200)

        record(dmm13,1,600,filename=Filenam,async = True)
        traces._figlist[-1].destroy()

        if sweep_S2V:
            name1="S2V"+Filename
            name2="dS2dV"+Filename
            name3="BateauNoise"+Filename
        
            set(moy_dif.moydev,-2.4e-3)
            set(moy_dif.difdev,0)
            wait(3)
        
            sweep(moy_dif.moydev,-2.4e-3,2.4e-3,141,filename=name1,out=dmm13,updown=True,close_after = True)
            #sweep(moy_dif.moydev,-2.4e-3,2.4e-3,141,filename=name2,out=sr2,updown=True,close_after = True,async=True)
            set(moy_dif.moydev,0)
            wait(3)
            set(moy_dif.difdev,-10e-3)
            wait(3)
            sweep(moy_dif.difdev,-10e-3,10e-3,141,filename=name3,out=dmm13,updown=True,close_after = True)
            set(moy_dif.difdev,0)
            set(moy_dif.moydev,0)
  
    for Temp in Temperatures[::-1]:
        if Temp<=0.02:
                set(tc3.heater_range,0.001)	
        if Temp>0.02 and Temp<=0.2:
                set(tc3.heater_range,0.00316)
        elif Temp>0.2:
                set(tc3.heater_range,0.01)

        if Temp<0.010:
                Filename = "_00{0}mK_%T_.txt".format(int(round(Temp*1000,-1)))
        elif Temp<0.100 and Temp>=0.010:
                Filename = "_0{0}mK_%T_.txt".format(int(round(Temp*1000,-1)))
        elif Temp>=0.100:
                Filename = "_{0}mK_%T_.txt".format(int(round(Temp*1000,-1)))

        Filenam = "calibration_dn" + Filename
        set(tc3.sp,Temp)

        # Get to temperature.
        temp_cryo=get(tc3.fetch) 

        while(temp_cryo[6]>Temp):
                wait(10)
                temp_cryo=get(tc3.fetch)

        # Wait for stabilisation. 
        wait(1200)

        record(dmm13,1,600,filename=Filenam, async = True)
        traces._figlist[-1].destroy()
      
        if sweep_S2V:
            name1="S2V"+Filename
            name2="dS2dV"+Filename
            name3="BateauNoise"+Filename
        
            set(moy_dif.moydev,-2.4e-3)
            set(moy_dif.difdev,0)
            wait(3)
        
            sweep(moy_dif.moydev,-2.4e-3,2.4e-3,141,filename=name1,out=dmm13,updown=True,close_after = True)
            #sweep(moy_dif.moydev,-2.4e-3,2.4e-3,141,filename=name2,out=sr2,updown=True,close_after = True,async=True)
            set(moy_dif.moydev,0)
            wait(3)
            set(moy_dif.difdev,-10e-3)
            wait(3)
            sweep(moy_dif.difdev,-10e-3,10e-3,141,filename=name3,out=dmm13,updown=True,close_after = True)
            set(moy_dif.difdev,0)
            set(moy_dif.moydev,0)
            
    sweep_S2V = False   
    
set(moy_dif.moydev,0)
set(tc3.heater_range,0)
set(tc3.sp,0.001)
