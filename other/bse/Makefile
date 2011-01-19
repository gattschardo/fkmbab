src:= \
	`find . -name '*.java' | grep src | grep -v server`

	#client-checkout/src/xways/client/visibleobjectcalculation/SlowFrameCalculator.java \
	#client-checkout/src/xways/client/userinterface/utils/ToggleActionsOnListSelect.java \
	#shared-checkout/src/xways/shared/mapmodel/TwoDTreeNode.java \
	#shared-checkout/src/xways/shared/mapmodel/TwoDTree.java \
	#converter-checkout/src/xways/converter/GeoMapper.java \
	#client-checkout/src/xways/client/visibleobjectcalculation/TwoDTreeFrameCalculator.java \
	#converter-checkout/src/xways/converter/ConverterController.java \
	#tools-checkout/src/xways/tools/TwoDTreeVerifier.java \
	#tools-checkout/src/xways/tools/NearestNode.java \
	#tools-checkout/src/xways/tools/RangeSearch.java \
	#client-checkout/src/xways/client/userinterface/MainWindow.java \
	#client-checkout/src/xways/client/controller/GUIMediator.java \
	#client-checkout/src/xways/client/userinterface/RoutePlanningPanel.java \
	#client-checkout/src/xways/client/userinterface/LeftBar.java \
	#client-checkout/src/xways/client/routecalculation/ShortestWayCalculator.java

cksrc:= \
	client-checkout/test/xways/client/visibleobjectcalculation/test/SlowFrameCalculatorTest.java \
	shared-checkout/test/xways/shared/mapmodel/test/TwoDTreeTest.java \
	converter-checkout/test/xways/converter/test/GeoMapperTest.java \
	client-checkout/test/xways/client/userinterface/utils/test/MapRenderingPanelTest.java \

	#client-checkout/test/xways/client/userinterface/utils/test/ToggleActionsOnListSelectTest.java

checks:= check-sfc \
	 check-2dtr \
	 check-geo

junit:=java org.junit.runner.JUnitCore

all:
	javac $(src)

check: ck-compile $(checks)

clean:
	find . -name '*.class' | xargs rm

ck-compile:
	javac $(cksrc)

# Individual calls to JUnit:

check-2dtr:
	$(junit) xways.shared.mapmodel.test.TwoDTreeTest

check-sfc:
	$(junit) xways.client.visibleobjectcalculation.test.SlowFrameCalculatorTest

check-geo:
	$(junit) xways.converter.test.GeoMapperTest
