import sys,json

if __name__ == '__main__':

	fileName = sys.argv[1]
	f = file(fileName)
	content = f.read()

	tmpCount = 0
	tmpContent = ''

	for ch in content:
		tmpContent = tmpContent + ch;
		if ch == '{':
			tmpCount = tmpCount + 1
		elif ch == '}':
			tmpCount = tmpCount - 1
		if tmpCount == 0:
			parseContent = tmpContent.strip();
			if parseContent:
				print json.dumps(eval(parseContent))
				tmpContent = ''



