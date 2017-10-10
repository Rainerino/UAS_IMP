#ifndef DCNC_HPP
#define DCNC_HPP

//===================================================================
// Includes
//===================================================================
// System Includes
#include <QString>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <memory>
// GCOM Includes
#include "modules/uas_message/uas_message.hpp"
#include "modules/uas_message/uas_message_tcp_framer.hpp"

//===================================================================
// Public Class Declaration
//===================================================================
/*!
 * \brief The DCNC class is designed to handle all communications between the
 *        GCOM and the Gremlin.
 * \details The DCNC has 3 states:
 *              1. OFFLINE
 *              2. SEARCHING
 *              3. CONNECTED
 *          The DCNC can always starts at OFFLINE and can transition between
 *          the states in the following ways:
 *          OFFLINE <--> SEARCHING <--> CONNECTED
 *          OFFLINE <-- CONNECTED
 */
class DCNC : public QObject
{
    Q_OBJECT

public:
    // Enum Definitions
    /*!
     * \brief The DCNCStatus enum lists all the possible states that the DCNC
     *        could be in
     */
    enum class DCNCStatus : int
    {
        OFFLINE         = 0,
        SEARCHING       = 1,
        CONNECTED       = 2
    };

    // Public Member Methods
    /*!
     * \brief DCNC constructor
     */
    explicit DCNC();

    /*!
     * \brief DCNC destructor
     */
    ~DCNC();

    // Server Control Methods
    /*!
     * \brief startServer, starts the DCNC server and moves its state to the
     *        SEARCHING state, where it will listen on the specified network
     *        interfaces
     * \param address, The address of the interface to listen on, as a QString
     * \param port, The port to bind the server to
     * \return true or false based on the success of binding the server
     */
    bool startServer(QString address, int port);

    /*!
     * \brief startServer, starts the DCNC server and moves its state to the
     *        SEARCHING state, where it will listen on ALL available network
     *        interfaces
     * \param port, The port to bind the server to
     * \return true or false based on the success of binding the server
     */
    bool startServer(int port);

    /*!
     * \brief stopServer, Stops the server and moves it from whatever state it
     *                    in to the OFFLINE state. If there were any connection
     *                    then they are automatically dropped.
     */
    void stopServer();

    /*!
     * \brief status, Allows other objects to query the status of the DCNC
     * \return an DCNCStatus enum value denoting the current state of the server
     */
    DCNCStatus status();

    // Connection Control Methods
    /*!
     * \brief cancelConnection, Drops the current connection and moves the DCNC
     *        back into SEARCHING mode
     */
    void cancelConnection();

    // Data Methods
    bool startImageTransfer();
    bool stopImageTransfer();

signals:
    // DCNC Control Signals
    void receivedConnection();
    void droppedConnection();
    // Data Signals
    void receivedImageData();

private slots:
    /*!
     * \brief handleConection is a slot that gets notified whenever a new
     *        connection is received.
     */
    void handleClientConection();

    /*!
     * \brief droppedConnection sets the system variables in the case we lose
     *        the client
     */
    void handleClientDisconnected();

    /*!
     * \brief handleMessage parses the message it receives and carries out tasks
     *        accordingly
     */
    void handleClientData();

private:
    // Private Member Variables
    int port;
    QString address;
    QHostAddress hostAddress;
    QTcpServer *server;
    QTcpSocket *clientConnection;
    QDataStream connectionData;
    std::unique_ptr<UASMessage> message;
    UASMessageTCPFramer messageFramer;
    DCNCStatus serverStatus;
};

#endif // DCNC_HPP
