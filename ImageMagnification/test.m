clear, close all;


% 
% t = 0:pi/50:10*pi;
% plot3(sin(t),cos(t),t)
% grid on
% axis square

% Y = [
%      80  0 60 0  120 0 150
%      0   0 0  0  0   0 0 
%      40  0 90 0  80  0 80
%      0   0 0  60 0   0 0
%      70  0 30 0  50  0 120
%      0   0 0  0  0   0 0
%      15   0 10 0 90 0 50
%     ];

% Y = [
%       0  0  0  0    0
%       0  110 0  150  0
%       0  0  75 0    0
%       0  20 0  50   0
%       0  0  0  0    0
%      ];
% 
% bar3(Y);

% [X,Y] = meshgrid([0:1:3]);
% 
%  
% Z = X + [1];
% %Z = [0 0 0 0; 0 5 0 0; 0 0 0 0];
% 
% plot3(X,Y,Z)
% 
% grid on

% i = imread( 'letter.bmp' );
% %  
% i = rgb2gray( i );
% 
% i = imresize( i, 0.1, 'bicubic' );
%  
% %plottools;
% 
% %i = peaks;
% plot(i,'YDataSource','i');
% grid on
% axis square

%Z = magic(5);

% j = [0 1 ; 1 1];
% 
% k = [0 0 1; 0 1 1; 1 1 1];
% 
% figure, bar3(j);
% figure, bar3(k);
% grid on
% axis square

%grid on;
%figure, imshow( i );

%i = wiener2( i );

%figure, imshow( i );


%imwrite( i, 'filtered.bmp', 'bmp' );

i = imread( 'uni.bmp' );


j = imresize( i, 2, 'nearest' );
k = imresize( i, 2, 'bilinear' );
l = imresize( i, 2, 'bicubic' );
% m = imresize( i, 2, 'box' );
n = imresize( i, 2, 'triangle' );
% o = imresize( i, 2, 'lanczos3' );
% 

% j = imresize( i, 4, 'nearest' );
% k = imresize( i, 4, 'bilinear' );
% l = imresize( i, 4, 'bicubic' );
% m = imresize( i, 4, 'box' );
% n = imresize( i, 4, 'triangle' );
% o = imresize( i, 4, 'lanczos3' );
% 
imwrite( j, 'x2nearestfootball.bmp', 'bmp' );
imwrite( k, 'x2bilinearfootball.bmp', 'bmp' );
imwrite( l, 'x2bicubicfootball.bmp', 'bmp' );
%imwrite( m, 'x2box.jpg', 'jpg' );
imwrite( n, 'x2srnefootball.bmp', 'bmp' );
%imwrite( o, 'x2lanczos.jpg', 'jpg' );




