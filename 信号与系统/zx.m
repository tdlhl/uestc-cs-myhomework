clear
[y1,fs]=audioread('lz.wav');
y1=y1(:,1);
N1=length(y1);
n=0:N1-1;
t=n/fs;
f=0:fs/N1:fs*(N1-1)/N1;
Y1=fft(y1,N1);

figure(1)
subplot(2,2,1);
plot(t,y1);
xlabel('时间t');
ylabel('幅度');
title('原信号时域波形图');
subplot(2,2,2);
plot(f,abs(Y1));
xlabel('频率f');
ylabel('幅度');
title('原信号频谱图');
y2=voice(y1,1);                               %1为f初始值                                   
N2=length(y2);
k=0:1:N2-1;
t=k/fs;
f=k*fs/N2;
Y2=fft(y2,N2);

subplot(2,2,3);
plot(t,y2);
xlabel('时间t');
ylabel('幅度');
title('变声信号时域波形图');
subplot(2,2,4);
plot(f,abs(Y2));
xlabel('频率f');
ylabel('幅度');
title('变声信号频谱图');

sound(y2,fs);                         %播放变声后的音频，若需关闭请在命令行窗口输入‘clear sound’
filename = '梁静茹-张信哲-梁静茹.wav';
audiowrite(filename,y2,fs);            %将变声后的音频以wav格式输出
 


function Y=voice(y1,f)  
f=round(f*910);                            %改变n即可实现声音变化，初始值1000.
d=resample(y1,f,1000);    
W=400;
Wov=W/2;
Kmax=W*2;
Wsim=Wov;
xdecim=8;
kdecim=2;
X=d';
F=f/1000;                      %改变F时长规整因子可改变语速，变大语速加快，反之减慢
Ss=W-Wov;
xpts=size(X,2);
ypts=round(xpts/F);
Y=zeros(1,ypts);
xfwin=(1:Wov)/(Wov+1);
ovix=(1-Wov):0;
newix=1:(W-Wov);
simix=(1:xdecim:Wsim)-Wsim;
padX=[zeros(1,Wsim),X,zeros(1,Kmax+W-Wov)];
Y(1:Wsim)=X(1:Wsim);
lastxpos=0;
km=0;
for ypos=Wsim:Ss:(ypts-W)
xpos=round(F*ypos);
kmpred=km+(xpos-lastxpos);
lastxpos=xpos;
if(kmpred<=Kmax)
   km=kmpred;
else
ysim=Y(ypos+simix);
rxy=zeros(1,Kmax+1);
rxx=zeros(1,Kmax+1);
Kmin=0;
for k=Kmin:kdecim:Kmax
xsim=padX(Wsim+xpos+k+simix);
rxx(k+1)=norm(xsim);
rxy(k+1)=(ysim*xsim');
end
Rxy=(rxx~=0).*rxy./(rxx+(rxx==0));
km=min(find(Rxy==max(Rxy))-1);
end
xabs=xpos+km;
Y(ypos+ovix)=((1-xfwin).*Y(ypos+ovix))+(xfwin.*padX(Wsim+xabs+ovix));
Y(ypos+newix)=padX(Wsim+xabs+newix);
end
end