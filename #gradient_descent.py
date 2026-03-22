#gradient_descent
def gradient_descent(X,y,theta,learing_rate,n_iterations):
    m=len(y)
    loss_history=[]
    for i in range(n_iterations):
        gradients=(1/m)*X.T.dot(X.dot(theta)-y)
        theta=theta-learing_rate*gradients
        loss=compute_loss(X,y,theta)
        loss_history.append(loss)
        if np.isnan(loss):break
    return theta,loss_history
    