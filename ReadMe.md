### ofxFastIOImage

Simple OF addon to load / save ofImages byte-to-byte as binary files, instead of traditional png / jpg approach. I found that saving png's using ofImage's load / save methods is especially slow on iOS. This add-on might speed up save/load for platforms with slow cpu and fast i/o. Of course the downside is that the files are not readable by anything else, and they are much bigger.

Requires ofxXmlSettings, and the example uses ofxTimeMeasurements to measure times. 

On an iPhone 4s, ofxFastIOImage is 2x faster at loading and 5x faster at saving than the usual ofImage methods.

![ofxFastIOImage screenshot](http://farm9.staticflickr.com/8428/7622699128_e39386169f_o.png)


