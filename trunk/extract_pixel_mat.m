function extract_pixel_mat()

	I = imread('blackx.bmp');
	[m,n] = size(I);
	dlmwrite('pixelmatrix.txt', I, 'delimiter', '\t');
	T = zeros(m,n);

	for i=1:m
		for j=1:n
			if( I(i,j) <= 127 )
				T(i,j) = 0;
			else
				T(i,j) = 255;
			end
		end
	end

	dlmwrite('thresholdpixelmatrix.txt', T, 'delimiter', '\t');

end
