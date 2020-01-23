# Virtual intruments

# temperature_control_sample
class lakeshore_370_logical(instruments.lakeshore_370):
    """ 
        lakeshore_370_logical is a logical device derived from lakeshore_370
        
        If temperatures out of the intended range are used it defaults to 
        cooldown (i.e. self.heater_range is set to 0).
    """
    def __init__(self, visa_addr ='ASRL1' ):
        super(lakeshore_370_logical, self).__init__(visa_addr)
        self.current_ch.set(6); # The 6th channel is the temperature of the sample
        print " \n Current channel is : {} \n and has temperature = {} K".format( self.current_ch.get(), self._current_ch_temperature_getdev() )
         
        # These tables could be fine tuned
        self.temperature_range_table =   [ 0.007 , 0.020     , 0.050 , 0.100    , 0.200     ]; # The upperbound of the interval
        self.temperature_tolerance =     [ 0.0   , 0.001     , 0.001 , 0.001    , 0.001     ]; # The upperbound of temperature error after stabilization_loop
        self.time_tolerance =            [ 0.0   , 60        , 60    , 60       , 60        ]; # The amount of time the temperature error as to be below the temperature_tolerance to say the stabvilization is acheived
        self.heater_range_table =        [ 0.0   , 0.000316  , 0.001 , 0.00316  , 0.01      ]; # see instruments.lakeshore_370.heater_range for all possible values
        self.proportional_table =        [ 25    , 25        , 25    , 12.5     , 6.25      ];
        self.integral_table =            [ 120   , 120       , 120   , 120      , 120       ];
        self.derivative_table =          [ 0     , 0         , 0     , 0        , 0         ];
        
        # PID For each range ??
        
        self._devwrap("stabilize_T_to", doc = "Virtual device to control temperature of the curtrent channel (ch = 6 by default), while respecting some pre-establishe controle tables")
        self._create_devs_helper(once=True)
        
        
    def get_table_index(self,T):
        t_table = self.temperature_range_table;
        index = 0;
        if ( ( T<=t_table[-1] ) and ( T>t_table[0] ) ) != True : print "Temperature outside of heater_range_table" ;  return index ; 
        for i , t in enumerate(t_table):
            if ( (T <= t) == True) :
                index = i ; 
                break ;
        return index ;
            
    def _stabilize_T_to_setdev(self,T):
        self.set_heater_range(T)
        set(self.sp,T)
        self.stabilization_loop(T)
        
    def _stabilize_T_to_getdev(self):
        # Returns current channel temperature
        return get(self.t)
        
    
    def stabilization_loop(self, T_target):
        """ 
        Stabilization is acheived when we have been in a 1 mK neighborhood 
        of the target temperature for a 60 sec.
        """
        print "starting stabilization_loop"
        t_tol = self.temperature_tolerance[self.get_table_index(T_target)];
        time_tol = self.time_tolerance[self.get_table_index(T_target)];
        T = self._current_ch_temperature_getdev();
        delta_T = abs(T - T_target);
        
        keep_going = True;
        converged = False;
        
        delta_T>=t_tol
        
        while( keep_going ):
            t_0 = time.time(); t_1 = t_0;
            print "first while"
            while( delta_T<=t_tol ):
                print "second while"
                wait(5)
                T = self._current_ch_temperature_getdev() ;
                delta_T = abs(T - T_target);
                t_1 = time.time();
                if (delta_T<=t_tol)==False :
                    break ;
                elif (t_1-t_0)>=time_tol :
                    converged = True;
                    break ;
            if converged == True : print "ending stabilization_loop" ; break
            wait(10)
            T = self._current_ch_temperature_getdev() ;
            delta_T = abs(T - T_target);
         
    def set_heater_range(self, T):
        index = self.get_table_index(T) ;
        # set_heater
        set(self.heater_range,self.heater_range_table[index]) ;
        # set PID parameters
        set(self.pid, P = proportional_table[index]);
        set(self.pid, I = integral_table[index]);
        set(self.pid, D = derivative_table[index]);
        

