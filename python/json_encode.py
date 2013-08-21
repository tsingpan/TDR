import json
from hotpot.hp_config import *
from hotdata.syntactic_node import *

def hpmain(document, target_dir):
	print(json.dumps(document))
