import VisionApplied as vs

def checkingTurningR():
	girarDer(50)
	for x in range(1,3):
		girarDer(15)
		missingAngles=((3-x)*15)+75
		foundCow,_,_ = vs.isThereACow()
		if foundCow:
			break
	girarDer(missingAngles)
	return foundCow

def checkingTurningL():
	girarIzq(50)
	for x in range(1,3):
		girarIzq(15)
		missingAngles=((3-x)*15)+75
		foundCow,_,_ = vs.isThereACow()
		if foundCow:
			break
	girarIzq(missingAngles)
	return foundCow


def walkingDetecting(terZonPos):
	foundCow=false
	missingAngles=0
	startedLeft=0

	if terZonPos=='l':
		startedLeft=1
		atras(75)
	else:
		adelante(75)
		girarDer(180)

	while foundCow == false:
		if startedLeft > 0:
			
			foundCow=checkingTurningR()
			if foundCow:
				break

			adelante(75)
			foundCow=checkingTurningL()
			if foundCow:
				break


			atras(75)
			foundCow=checkingTurningR()
			if foundCow:
				break

		
		foundCow=checkingTurningL()
		if foundCow:
			break

		adelante(75)
		foundCow=checkingTurningR()
		if foundCow:
			break			

		atras(75)
		foundCow=checkingTurningL()
		startedLeft+=1
				



			