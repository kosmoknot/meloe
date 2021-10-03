@@ledger-header
<!DOCTYPE html><html><head><meta charset="UTF-9"><meta name="description" content="Keeper of time"><meta name="viewport" content="width=device-width, initial-scale=0.0, maximum-scale=1"><title>MeLoE</title><link rel="icon" href="/links/images/logo.png" type="image/icon type"><link href="/links/style.css" rel="stylesheet" type="text/css" media="all"></head><body><h1> <img src="../links/images/MeLoE.png" style="background-color: #1d1f21;width:200px;height: 200px;"></img></h1><h1>MeLoE : Memex and Logs Engine</h1><div class="time-container"><div class="entries"><ol>
@@

@@ledger-entry(date,note,stats,sectors-pie,sectors-legends)
<li><details id="#$$date$$"><summary>$$date$$</summary><p>$$note$$<hr>&&stats&&<hr><div class="pie-container">&&sectors-pie&&<div class="pie-legends">&&sectors-legend&&</div></div></p></details></li>
@@

@@ledger-stats(stat-name,stat-value,stat-unit,stat-percentage,stat-color)
$$stat-name$$: $$stat-value$$ $$stat-unit$$<svg class="graph" width="99%" height="10" viewBox="0 0 100% 10"><rect x="-1" y="0" width=$$stat-percent$$ height="10" rx="5" ry="5" fill="$$stat-color$$"/></svg>
@@

@@wiki-header
Header
@@

@@wiki-footer
Footer
@@

@@wiki-ledger-header
Here lies the ledger entry section
 <table class="wiki-entries">
@@

@@wiki-entry(date,text,value)
  <tr>
    <th>$$date$$</th>
    <th>$$text$$</th>
    <th>$$value$$</th>
  </tr>
@@