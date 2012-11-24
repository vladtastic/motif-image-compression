function [blanked_image] = image_blanker( img, xstart, xend, ystart, yend )

	blanked_image = img;

	[m,n] = size(blanked_image);

	for i=xstart:xend
		for j=ystart:yend
			blanked_image(i,j) = -1;
		end
	end

	dlmwrite('blanked_image.txt', blanked_image, '\t');

end
