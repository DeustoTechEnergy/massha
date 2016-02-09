import sys
import os
import csv
import json
import copy
import random
import datetime
from collections import OrderedDict

# Retrieve current execution path
path = os.path.dirname( os.path.abspath(__file__) )

# Retrieve console parameters
path_conf = os.path.join( path, sys.argv[1] )
path_schedule = os.path.join( path, sys.argv[2] )
dataset = sys.argv[3]

# Initialize seed 
random.seed()

activity_eqvs = { 
		  # KITCHEN
		  'Cleaning': 'CLEANING',
		  'Preparing a beverage': 'PREPARING_BEVERAGE',
		  'Preparing breakfast': 'PREPARING_BREAKFAST',
		  'Preparing dinner': 'PREPARING_DINNER',
		  'Preparing lunch': 'PREPARING_LUNCH',
		  'Preparing a snack': 'PREPARING_SNACK',
		  'Putting away dishes': 'PUTTING_AWAY_DISHES',
		  'Putting away groceries': 'PUTTING_AWAY_GROCERIES',
		  'Putting away laundry': 'PUTTING_AWAY_LAUNDRY',
		  'Taking medication': 'TAKING_MEDICATION',
		  'Washing dishes': 'WASHING_DISHES',
		  # BATHROOM
		  'Bathing': 'BATHING',
		  'Doing laundry' : 'DOING_LAUNDRY',	
		  'Dressing': 'DRESSING',
		  'Grooming': 'GROOMING',
		  'Toileting' : 'TOILETING',
		  'Washing hands' : 'WASHING_HANDS',
		  # LIVING ROOM
		  'Home education': 'HOME_EDUCATION',
		  'Listening to music': 'LISTENING_MUSIC',	
		  'Talking on telephone': 'TALKING_ON_TELEPHONE',
		  'Watching TV': 'WATCHING_TV',
		  'Working at computer': 'WORKING_AT_COMPUTER',
		  # FEYER
		  'Going out for shopping': 'GO_OUT_SHOPPING',		
		  'Going out for entertainment': 'GO_OUT_ENTERTAINING',
		  'Going out to work' : 'GO_OUT_TO_WORK',	
		  'Lawnwork': 'LAWNWORK',
		  'Other': 'OTHER',		  
}

subactivity_eqvs = { 
		      # GENERAL
		      'Door': 'USE_DOOR',
		      'Lightswitch': 'USE_SWITCH',
		      # KITCHEN
		      'Burner': 'USE_BURNER',
		      'Cabinet': 'USE_CABINET',
		      'Cereal' : 'USE_CEREAL',
		      'Coffeemachine' : 'USE_COFFEE_MACHINE',
		      'Containers': 'USE_CONTAINER',
		      'Dishwasher' : 'USE_DISHWASHER',
		      'Drawer': 'USE_DRAWER',
		      'Freezer': 'USE_FREEZER',
		      'Garbagedisposal': 'USE_GARBAGE_DISPOSAL',
		      'LaundryDryer': 'USE_LAUNDRY_DRIER',
		      'Medicinecabinet': 'USE_MEDICINE_CABINET',				      
		      'Microwave': 'USE_MICROWAVE',
		      'Oven' : 'USE_OVEN',		   		      
		      'Refrigerator': 'USE_REFRIGERATOR',
		      'Toaster': 'USE_TOASTER',
		      # BATHROOM
		      'Hamper': 'USE_HAMPER',
		      'Sinkfaucet-cold': 'USE_SINK_FAUCET_COLD',
		      'Sinkfaucet-hot': 'USE_SINK_FAUCET_HOT',		      
		      'Showerfaucet': 'USE_SHOWER_FAUCET', 
		      'ToiletFlush': 'USE_TOILET_FLUSH',
		      'WashingMachine': 'USE_WASHING_MACHINE',
		      'Closet' : 'USE_CLOSET',		      
		      # LIVING ROOM
		      'DVD' : 'USE_DVD',		      
		      'ExhaustFan': 'USE_FAN',
		      'Lamp': 'USE_LAMP',		      		     		      
		      'Jewelrybox': 'USE_JEWELRY_BOX',
		      'Stereo': 'USE_STEREO',
		      'Telephone': 'USE_TELEPHONE',
		      'TV': 'USE_TV',
}  

unnatended_activities = [ subactivity_eqvs['DVD'],
			  subactivity_eqvs['ExhaustFan'],
			  subactivity_eqvs['Lamp'],
			  subactivity_eqvs['LaundryDryer'],
			  subactivity_eqvs['Stereo'],
			  subactivity_eqvs['TV'],
			  subactivity_eqvs['ToiletFlush'],
			  subactivity_eqvs['Lightswitch'],
			  subactivity_eqvs['Door']			  			  
			]

mandatory_activities = { activity_eqvs['Going out for shopping']: 1,
			 activity_eqvs['Going out for entertainment']: 4,
			 activity_eqvs['Going out to work']: 4  
		       }


important_activities = [ activity_eqvs['Preparing breakfast'],
			 activity_eqvs['Preparing lunch'],
			 activity_eqvs['Preparing dinner']  
		       ]

print 'Parsing file in {0}'.format( path_conf )

      
# **********************************************************************      
#                           PARSE ACTIVITIES
# **********************************************************************

fs = open( path_conf, 'rb' ) # opens the csv file
try:
    activities = []
    activity = {}
    activity_name = ''
    reader = csv.reader(fs, delimiter=',')  # creates the reader object
    
    for row in reader:       
      if len(row) == 1:
	if 'count' not in row[0] and 'sensors' not in row[0]:
	  activity = {}
	  activity['activity_type'] = activity_eqvs[row[0]] 
	  activity['attendance'] = 'ATTENDED'
	  activity['finish_objective'] = 100
	  activity['priority'] = 0
	  activity['priority_increment'] = 0
	  activity['subactivities'] = []
	  activities.append(activity)
	elif 'count' in row[0]:
	  activity['count'] = float(row[0].replace('count: ', '')) 
      else:			
	sensor_name = row[0].replace(' ', '').replace("\t", '')
	subactivity_type = sensor_name.strip().split('_')[0]
	frequency = row[1].replace(' ', '').replace("freq:", '')
	duration = row[2].replace("duration: ( ",'').replace(" )", '').strip().split(" ")
	duration_mean = duration[0]
	duration_deviation = duration[1]	
		
	subactivity = {}
	subactivity['activity_type'] = subactivity_eqvs[subactivity_type]
	subactivity['sensor_name'] = sensor_name	
	subactivity['attendance'] = 'UNATTENDED' if subactivity_eqvs[subactivity_type] in unnatended_activities else 'ATTENDED'	
	subactivity['frequency'] = float( frequency )
	subactivity['duration_mean'] = float( duration_mean )
	subactivity['duration_deviation'] = float( duration_deviation )
	subactivity['finish_increment'] = 5
		
	activity['subactivities'].append(subactivity)
finally:   
  fs.close()
  
# ADD WAKE_UP ACTIVITY  
activity = {}
activity['activity_type'] = 'WAKE_UP'
activity['attendance'] = 'ATTENDED'
activity['finish_objective'] = 0
activity['mandatory_start_time'] = '06:30'
activity['mandatory_end_time'] = '06:45'
activity['priority_base'] = 0
activity['priority_increment'] = 20
activity['duration_mean'] = 100
activity['subactivities'] = []
activities.append(activity)  
  
  
# **********************************************************************      
#              PARSE SCHEDULE TO BUILD TO_DO LIST
# **********************************************************************  

schedule = {}
for i in range(0, 24):
  schedule[i] = {}

# Parse schedule file  
fs = open( path_schedule, "rb" )
try:
  reader = fs.readlines()
  reading_frequencies = True

  for line in reader:  
    if line == '\n':
      continue  
    else:   
      if '/' in line:
	reading_frequencies = False   	  
      if reading_frequencies is False:               
	if '/' not in line:	
	  line = line.strip().replace(' ]', '').split(' [ ')
	  scheduled_activity =  activity_eqvs[line[0]] 
	  
	  if scheduled_activity != 'OTHER':	  
	    times = line[1].split(' , ')	
	    time_start = datetime.datetime.strptime(times[0], '%H:%M:%S')
	    time_end = datetime.datetime.strptime(times[1], '%H:%M:%S')
	    duration = (time_end - time_start).seconds	

	    if scheduled_activity not in schedule[time_start.hour].keys():
	      schedule[time_start.hour][scheduled_activity] = [0 , 0.0]
	    schedule[time_start.hour][scheduled_activity][0] += 1
	    schedule[time_start.hour][scheduled_activity][1] += duration	    
	      
finally:
  fs.close()
	
# Build roulette from schedule frequencies
roulette = {}
for h in schedule.keys():  
  roulette[h] = OrderedDict()
  for sa in schedule[h].keys():
    schedule[h][sa][1] = round(schedule[h][sa][1] / schedule[h][sa][0], 2)    
  for sa in schedule[h].keys():  
    roulette[h][sa] =  schedule[h][sa][0]
  roulette[h] = OrderedDict(sorted(roulette[h].items(), key=lambda t: t[1]))
	    

  # Build TO_DO list from roulette
def create_todo():
  todo_list = []
  first_activity = True
  timestamp = datetime.datetime.strptime('06:40', '%H:%M')

  for h in range(6, 24):
    total_frequencies = 0
    possible_activities = schedule[h]    
    
    if timestamp.hour > h:
      continue
    
    activities_num = len(roulette[h].values())
    pick_num = int(random.uniform(1, activities_num+1))
      
    for i in range(random.randint(0,3)):
      roulette_activity = {}
      # Calculate seconds between
      seconds_between = random.uniform(0.0, 1800)
      timestamp += datetime.timedelta(seconds=seconds_between)

      # Select random   
      activity_pick = random.uniform(0.0, sum(roulette[timestamp.hour].values()))  
            
      # Find activity that falls within interval
      for idx, value in enumerate(roulette[h].values()):
	activity = {}
	total_frequencies += value    
	
	if total_frequencies > activity_pick: 	  	    	    
	  roulette_activity = roulette[h].keys()[idx]		    
	  
	  # Get start and end times of chosen activity
	  activity_stime = timestamp.strftime('%H:%M')
	  activity_duration = schedule[h][roulette_activity][1]	  	  
	  
	  activity['activity_type'] = roulette_activity
	  
	  if roulette_activity in mandatory_activities:
	    timestamp += datetime.timedelta(hours=mandatory_activities[roulette_activity]) 
	    activity_etime = timestamp.strftime('%H:%M')
	    activity['mandatory_start_time'] = activity_stime	    
	    activity['mandatory_end_time'] = activity_etime 	    
	  
	  else:
	    # Add duration of activity
	    timestamp += datetime.timedelta(seconds=activity_duration) 
	    activity_etime = timestamp.strftime('%H:%M')	    
	    activity['desired_start_time'] = activity_stime
	    activity['desired_end_time'] = activity_etime
	    
	  activity['preconditions'] = []
	  activity['preconditions'].append('WAKE_UP')	   	  
	  todo_list.append(activity)
	  break   
	  
  return todo_list
	
  
# **********************************************************************      
#                 BUILD ORIGINAL_CONFIGURATION
# **********************************************************************     

path_dest = os.path.join( path, '{0}-s{1}.json'.format( 'activities', dataset ) )    
print 'Writing file in {0}'.format( path_dest )

  # Write to JSON file
fd = open(path_dest,'w')    
try:
  configuration = {}
  configuration['todo_list'] = create_todo()                 
  configuration['activity_configuration'] = activities
  fd.write( json.dumps( configuration, sort_keys=True, indent=4, separators=(',', ': ') ) )   
finally:
  fd.close() 
    
    
#**********************************************************************      
#   BUILD RANDOM CONFIGURATIONS WITH DIFFERENT MEANS AND DURATION 
#********************************************************************** 

for i in range(100):
  path_dest = os.path.join( path, 's' + dataset, 'configurations', '{0}-s{1}-c{2}.json'.format( 'activities', dataset, i ) )
  print 'Writing file in {0}'.format( path_dest )
  
  # Write to JSON file
  fd = open(path_dest,'w')    
  try:
    configuration = {}
    configuration['todo_list'] = create_todo()  
    activities_copy = copy.deepcopy(activities)    
    
    for a in activities_copy:
      for s in a['subactivities']:
	s['frequency'] = s['frequency'] + random.uniform(0, s['frequency'] * 0.10)
	s['duration_mean'] = s['duration_mean'] + random.uniform(0, s['duration_mean'] * 0.10)
                 
    configuration['activity_configuration'] = activities_copy
    fd.write( json.dumps( configuration, sort_keys=True, indent=4, separators=(',', ': ') ) )
    
  finally:
    fd.close() 