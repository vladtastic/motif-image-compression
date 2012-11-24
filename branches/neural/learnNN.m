function learnNN(X,Y)
[M,n] = size(X);

%Learning rate
eta_op = 0.0001;
eta_hid = 0.0001;

%Vector of predicted hypothesis values
h = zeros(M,1);

%Number of nodes in the input layer
N = n+1;
%Expand the X matrix by one column to account for the bias variable
x = ones(M,N);
x(:,2:N) = X(:,:);
x = x';

%The number of nodes in the hidden layer is P. P is hardcoded
P = 2;

%Declare theta  = NxP
theta = ones(N,P);
%Declare W = Px1
W = ones(P+1,1);

%Declare 'a' and 'z' for the hidden layer
a = zeros(P,M);
z = ones(P+1,M);

%Calculate initial error


J = 1000;

%Start the iteration
while J > 0.001
    %Calculate the a vector
    a = theta' * x;
    for i = 1:M
        %Calculate the sigmoid values of these functions
        for k =2:P+1
            z(k,i) = sigmoid(a(k-1,i));
        end
        h(i,1) = W'*z(:,i);
    end
    J = norm(h-Y,2)^2
    %%%%%%Calculate the gradients%%%%%%%%%%%%
    
    %Calculate the gradients for the Wk
    for k = 1:P+1
        Ek = 0;
        for i = 1:M
            Ek = Ek - (Y(i,1) - h(i,1))*z(k,i);
        end
        W(k,1) = W(k,1) - eta_op * Ek;
    end
    
    %calculate the gradients for thetaj
    for j = 1:N
        for k = 1:P
            Ekj = 0;
            for i = 1:M
                Ekj = Ekj - 2*(Y(i,1) - h(i,1)) * (W(k+1,1) * x(j,i) * sigmoid(a(k,i)) * (1-sigmoid(a(k,i))));
            end
            theta(j,k) = theta(j,k) - eta_hid * Ekj;
        end
    end
    
    
    
end
dlmwrite('C:\Users\Anagha\Documents\College\CSE 6740\Project\ml_project/theta.txt', theta, 'delimiter', '\t', '-append');
dlmwrite('C:\Users\Anagha\Documents\College\CSE 6740\Project\ml_project/w.txt', w, 'delimiter', '\t', '-append');

end