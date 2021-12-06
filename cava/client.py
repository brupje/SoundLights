import sys
import socket
import array


COLORS_COUNT = 256
COLORS_OFFSET = 50

MAX_COL_VAL = 32676
SEGMENTS = 30
NUM_BARS=144
SEGMENT_SIZE =int( NUM_BARS/SEGMENTS)


int_tracker=0

def get_spaced_colors(n):

   

  return [(int(color)) for color in  range(0, MAX_COL_VAL, 1)]


def send(colors):
    line = array.array('B', colors).tobytes()
    sock.sendto(line, ('255.255.255.255', 12345))


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

#colors = get_spaced_colors(COLORS_COUNT)
t_burst=0

prevTotal=0;

led_colors=[24 for i in range(0,NUM_BARS)]
led_colors.extend([0,0,0])
diff_above_100 =0

while True:
    try:
    
        for i in range(0,NUM_BARS):   
          led_colors[i] =  max(0,led_colors[i]-120)
    
    
        values = sys.stdin.readline()[:-1].split()
        nums =  [int(numeric_string) for numeric_string in values]
        num_above_min =0
                
        t_avg = 0
        t_highest=0
    
        t_total=0
        t_intensity =0
        
        for i in range(0,NUM_BARS):
          t_total += nums[i]
          
        t_avg =  t_total/NUM_BARS;
        factor=1  
        if t_avg >5:
          factor = 0.5
        if t_avg >10:
          factor = 0.1
        if t_avg >20:
          factor = 0.01
        t_diff =0
        for i in range(0,NUM_BARS):
          
            
          new_val = min(255,round(led_colors[i]+  nums[i]*nums[i]*nums[i]*factor))
          if (abs(new_val-led_colors[i]) >150):
            t_diff +=1
          
          led_colors[i] =new_val
          
          if led_colors[i]>200:
            num_above_min +=1


        if t_diff>50 and diff_above_100==0:
          t_burst=4
          diff_above_100=5
        else:
          diff_above_100=max(0,diff_above_100-1)
          

          
          
        if t_burst>0:
          t_burst-=1;
       
       
        if (num_above_min>100) and int_tracker<100 :
          t_burst =5
          
        t_speed =100
        if t_diff>50:
          t_speed=50
        if t_diff>100:
          t_speed=10
          
        t_intensity=num_above_min;
           
        prevTotal = t_total;    
       
        avg = t_total/NUM_BARS
   
        int_tracker +=t_intensity;
          
        int_tracker =int_tracker /1.5;
        
    
        #print (led_colors)
      
        print ("Diff: {:7.2f} Factor: {:3.2f} avg: {:8.2f} Intensity: {:6.2f} Tracker: {:8.2f} Speed: {:3} Num intese:{:3} Burst: {}".format(t_diff,factor,avg,t_intensity,int_tracker,t_speed,num_above_min,t_burst))
          
  
       
        
      
      
        led_colors[NUM_BARS+1]=t_burst;
        led_colors[NUM_BARS+2]=t_speed;
        
        send(led_colors)
        #print (led_colors)
    except Exception as e:
        print(e)



