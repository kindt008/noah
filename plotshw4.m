load mat.txt
import mlreportgen.dom.*
import DOM_LIST.*
[n,p]=size(mat);
t=1:n;
% make new curves to copy 6 with different window sizes  
%matsml=smoothdata(mat,'lowess');
matsms=smoothdata(mat,'sgolay',101);
%matsms2=smoothdata(smoothdata(mat,'sgolay'),'sgolay');
%matsms3=smoothdata(matsms2,'lowess');
%srts=UnorderedList();
%fshs=UnorderedList(); %using these lists is freaking impossible
ts=68; %value for border or variable regions can modify to desired value here
if matsms(1)<ts
    flag=1;
else
    flag=0;
end
%flag =1 if in variable region and flag=0 if not in variable region
%formatspec='begin: %d\tend: %d\n';
formatspec='%d\t';
fID=fopen('variable_regions.txt','w');


for i=1:n
    if flag==0 && matsms(i)>ts
        flag=1;
        fprintf(fID,'%d\t',i);
        fprintf(fID,'\n');
        %fsh=matsms(i); %print this value or save idk
        %append(fshs,i);
        %append(fshs,ListItem(i));
    elseif flag==1 && matsms(i)<ts
        flag=0;
        fprintf(fID,'%d\t',i);
        
        %srt=matsms(i); %print the value or save idk
        %append(srts,i);
        %append(srts,ListItem(i));
    elseif i==n && flag==0
        fprintf(fID,'%d\n',i);
    else
        continue
    end
end

fclose(fID);

figure(1)
plot(t,matsms,'g-',t,ts*ones(size(t)),'b-'),xlabel('location'),ylabel('% frequencey smoothed sgolay 101 window')
