# ICCIDGen
Generates euimid from iccids
<br/><br/>
Usage: iccidgen [-1] &lt;input csv&gt; [output csv]<br/>
&nbsp;&nbsp;-1<br/>
&nbsp;&nbsp;&nbsp;&nbsp;Use format version 1 (defaults to 2)<br/>
<br/><br/>
&nbsp;&nbsp;&lt;input csv&gt;<br/>
&nbsp;&nbsp;&nbsp;&nbsp;A comma-separated file, see Formats below for details<br/>
&nbsp;&nbsp;&nbsp;&nbsp;one per line.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;KI Key and Transport Key are optional<br/>
<br/>
&nbsp;&nbsp;[output csv]<br/>
&nbsp;&nbsp;&nbsp;&nbsp;Output file, where to write the data<br/>
&nbsp;&nbsp;&nbsp;&nbsp;optional and defaults to the console<br/>
<br/>
Formats<br/>
&nbsp;&nbsp;Version 1:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;Input<br/>
&nbsp;&nbsp;&nbsp;&nbsp;ICCID;KI Key;Transport Key<br/>
&nbsp;&nbsp;&nbsp;&nbsp;Output<br/>
&nbsp;&nbsp;&nbsp;&nbsp;ICCID;SF_EUIMID;pUIMID;Encrypted KI Key<br/>
&nbsp;&nbsp;Version 2:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;Input<br/>
&nbsp;&nbsp;&nbsp;&nbsp;ICCID;KI Key;Transport Key<br/>
&nbsp;&nbsp;&nbsp;&nbsp;Output<br/>
&nbsp;&nbsp;&nbsp;&nbsp;ICCID;SF_EUIMID;pUIMID;A12 CHAP;MN AAA<br/>