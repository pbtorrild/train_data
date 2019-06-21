//turn bineary
threshold( grad, grad, 127, 255,THRESH_BINARY);

//Opening ei. erosion then dialation:
int erosion_elem = 2;
int erosion_size = 1;
int dilation_elem = 2;
int dilation_size = 1;
// Set erostion type
int erosion_type;
switch (erosion_elem) {
  case 0:erosion_type = MORPH_RECT;break;
  case 1:erosion_type = MORPH_CROSS;break;
  case 2:erosion_type = MORPH_ELLIPSE;break;
}
// Set dialation type
int dilation_type;
switch (dilation_elem) {
  case 0:dilation_type = MORPH_RECT;break;
  case 1:dilation_type = MORPH_CROSS;break;
  case 2:dilation_type = MORPH_ELLIPSE;break;
}

Mat element_ero = getStructuringElement( erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                     Point( erosion_size, erosion_size ) );

Mat element_dil = getStructuringElement( dilation_type,Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                     Point( dilation_size, dilation_size ) );

erode( grad, grad, element_ero );
dilate( grad, grad, element_dil );
medianBlur( grad, grad, 25);
