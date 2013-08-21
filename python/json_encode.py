import json
from hotpot.hotpot.hp_config import *
from hotpot.hotdata.syntactic_node import *

def hpmain(document, target_dir):
	print(json.dumps(document))
