; Make List
@for /A:+d %dir in (*.*) do for %file in (%@name["%dir"]\render\psx\*.bmp) do @echo %@name["%dir"]\render\psx\%@name["%file"] >>a.txt

; Make Target Dirs
@for %File in (@a.txt) do if not exist %File md %File

; Move Files to Target Dirs
@for %File in (@a.txt) do if exist %File*.bmp move %File*.bmp %File




