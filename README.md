# ICCIDGen
Generates euimid from iccids
<br/><br/>
Usage: iccidgen &lt;input csv&gt; [output csv]<br/>
&nbsp;&lt;input csv&gt; is a comma-separated file with the format:<br/>
&nbsp;&nbsp;ICCID;KI Key;Transport Key<br/>
&nbsp;&nbsp;one per line.<br/>
&nbsp;&nbsp;KI Key and Transport Key are optional.<br/><br/>
&nbsp;[output csv] is optional and specifies where to write<br/>
&nbsp;&nbsp;the output, in the format: ICCID;SF_EUIMID;EUIMID<br/>
&nbsp;&nbsp;one per line.<br/>
&nbsp;&nbsp;If this is not specified it will write to the console.<br/>
