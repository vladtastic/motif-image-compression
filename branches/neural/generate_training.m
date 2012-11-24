function generate_training( filename )

	%	Parse an image

	I = imread( filename );
	I = rgb2gray( I );		% Pixel values are unsigned ints
    %imshow(I);              % Display the grayscale image
	I = double(I);			% Convert them to double - matlab works on doubles

	
	[m,n] = size(I);
    side = 10;
    
    %printf('Image has size %d by %d', m, n);
	
	%if( m < 1 || n < 1 )
	%	printf('Error in image dimensions\n');
	%	exit;
	%end

	X = zeros((m-side)*(n-side),side*side+1);	% last column are intensity values ("labels")
	y = zeros((m-side)*(n-side),1);
	stencil = zeros(side,side);
	t = 1;
for row_st = 1:(m-side)
        row_st
      for col_st = 1:(n-side)
			%Create the square stencil
			for i = 1:side
				for j = 1:side
						stencil(i,j) = I(row_st+i-1,col_st+j-1);
				end
			end
			%Assign elements of stencil to appropriate data structures
			y(t,1) = stencil(1,1);
			k = 1;
			for j=2:side
				X(t,k) = stencil(1,j);
				k = k+1;
			end
			for i=2:side
				for j=1:side
					X(t,k) = stencil(i,j);
					k = k+1;
				end
			end
			X(t,k) = row_st;
			X(t,k+1) = col_st;
			
			%Increment the index of the training example
			t = t+1;
      end
end
dlmwrite('C:\Users\Anagha\Documents\College\CSE 6740\Project\ml_project/X.txt', X, 'delimiter', '\t', '-append');
dlmwrite('C:\Users\Anagha\Documents\College\CSE 6740\Project\ml_project/y.txt', y, 'delimiter', '\t', '-append');


end