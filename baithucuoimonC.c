
#include <stdio.h>
#include <string.h>

#define MAX 100
#define MAX_ITEMS 20

typedef struct {
    char id[11];
    char name[41];
    char unit[11];
    double price;
    int quantity;
} Product;

typedef struct {
    char productId[11];
    char productName[41];
    int quantity;
    double price;
    double amount;
} InvoiceItem;

typedef struct {
    InvoiceItem items[MAX_ITEMS];
    int itemCount;
    double total;
    double discount;
    double finalTotal;
} Invoice;

Product products[MAX];
Invoice invoices[MAX];

int productCount = 0;
int invoiceCount = 0;
double revenue = 0;

// Tim san pham theo ma
int findProduct(char id[]) {
    int i;

    for (i = 0; i < productCount; i++) {
        if (strcmp(products[i].id, id) == 0)
            return i;
    }

    return -1;
}

// Nhap thong tin san pham
void inputProduct(Product *p) {
    do {
        printf("Ma san pham: ");
        scanf(" %10s", p->id);

        if (findProduct(p->id) != -1)
            printf("Ma da ton tai!\n");

    } while (findProduct(p->id) != -1);

    printf("Ten san pham: ");
    scanf(" %40[^\n]", p->name);

    printf("Don vi tinh: ");
    scanf(" %10[^\n]", p->unit);

    do {
        printf("Don gia: ");
        scanf("%lf", &p->price);

        if (p->price <= 0)
            printf("Don gia phai > 0!\n");

    } while (p->price <= 0);

    do {
        printf("So luong ton: ");
        scanf("%d", &p->quantity);

        if (p->quantity < 0)
            printf("So luong phai >= 0!\n");

    } while (p->quantity < 0);
}

// 1. Them san pham
void addProduct() {
    if (productCount >= MAX) {
        printf("Danh sach da day!\n");
        return;
    }

    inputProduct(&products[productCount]);
    productCount++;

    printf("Them thanh cong!\n");
}

// 2. Hien thi danh sach
void displayProducts() {
    int i;

    if (productCount == 0) {
        printf("Chua co san pham nao!\n");
        return;
    }

    printf("\n%-5s %-12s %-20s %-10s %-12s %-8s %-15s\n",
           "STT", "Ma", "Ten", "DVT",
           "Don gia", "SL", "Gia tri ton");

    for (i = 0; i < productCount; i++) {
        printf("%-5d %-12s %-20s %-10s %-12.2lf %-8d %-15.2lf\n",
               i + 1,
               products[i].id,
               products[i].name,
               products[i].unit,
               products[i].price,
               products[i].quantity,
               products[i].price * products[i].quantity);
    }
}

// 3. Tim san pham theo ma
void searchProduct() {
    char id[11];
    int index;

    printf("Nhap ma can tim: ");
    scanf(" %10s", id);

    index = findProduct(id);

    if (index == -1) {
        printf("Khong tim thay!\n");
    } else {
        printf("Ma: %s\n", products[index].id);
        printf("Ten: %s\n", products[index].name);
        printf("DVT: %s\n", products[index].unit);
        printf("Don gia: %.2lf\n", products[index].price);
        printf("So luong ton: %d\n", products[index].quantity);
        printf("Gia tri ton: %.2lf\n",
               products[index].price * products[index].quantity);
    }
}

// 4. Cap nhat gia va nhap them hang
void updateProduct() {
    char id[11];
    int index, quantity;

    printf("Nhap ma san pham: ");
    scanf(" %10s", id);

    index = findProduct(id);

    if (index == -1) {
        printf("Khong co ma san pham!\n");
        return;
    }

    do {
        printf("Nhap don gia moi: ");
        scanf("%lf", &products[index].price);
    } while (products[index].price <= 0);

    do {
        printf("Nhap so luong hang nhap them: ");
        scanf("%d", &quantity);

        if (quantity < 0 ||
            quantity > 1000000 - products[index].quantity)
            printf("So luong khong hop le!\n");

    } while (quantity < 0 ||
             quantity > 1000000 - products[index].quantity);

    products[index].quantity += quantity;

    printf("Cap nhat thanh cong!\n");
    printf("So luong ton moi: %d\n", products[index].quantity);
}

// 5. Xoa san pham
void deleteProduct() {
    char id[11];
    int index, i;

    printf("Nhap ma san pham can xoa: ");
    scanf(" %10s", id);

    index = findProduct(id);

    if (index == -1) {
        printf("Khong tim thay san pham!\n");
        return;
    }

    for (i = index; i < productCount - 1; i++) {
        products[i] = products[i + 1];
    }

    productCount--;

    printf("Xoa thanh cong!\n");
}

// 6. Ban hang va in hoa don
void sellProducts() {
    Invoice invoice;
    char id[11];
    int index, quantity, i, itemIndex, soldQuantity;

    if (invoiceCount >= MAX) {
        printf("Da dat gioi han luu hoa don!\n");
        return;
    }

    invoice.itemCount = 0;
    invoice.total = 0;
    invoice.discount = 0;
    invoice.finalTotal = 0;

    printf("\nNhap ma san pham (nhap 0 de ket thuc)\n");

    while (invoice.itemCount < MAX_ITEMS) {
        printf("Ma san pham: ");
        scanf(" %10s", id);

        if (strcmp(id, "0") == 0)
            break;

        index = findProduct(id);

        if (index == -1) {
            printf("Khong tim thay san pham!\n");
            continue;
        }

        printf("Ten: %s\n", products[index].name);
        printf("Don gia: %.2lf\n", products[index].price);
        printf("Ton kho: %d\n", products[index].quantity);

        printf("So luong mua: ");
        scanf("%d", &quantity);

        if (quantity <= 0) {
            printf("So luong phai > 0!\n");
            continue;
        }

        soldQuantity = 0;

        for (i = 0; i < invoice.itemCount; i++) {
            if (strcmp(invoice.items[i].productId, id) == 0)
                soldQuantity += invoice.items[i].quantity;
        }

        if (quantity > products[index].quantity - soldQuantity) {
            printf("Khong du hang trong kho!\n");
            continue;
        }

        itemIndex = -1;

        for (i = 0; i < invoice.itemCount; i++) {
            if (strcmp(invoice.items[i].productId, id) == 0) {
                itemIndex = i;
                break;
            }
        }

        if (itemIndex != -1) {
            invoice.items[itemIndex].quantity += quantity;
            invoice.items[itemIndex].amount =
                invoice.items[itemIndex].quantity *
                invoice.items[itemIndex].price;
        } else {
            strcpy(invoice.items[invoice.itemCount].productId,
                   products[index].id);

            strcpy(invoice.items[invoice.itemCount].productName,
                   products[index].name);

            invoice.items[invoice.itemCount].quantity = quantity;
            invoice.items[invoice.itemCount].price = products[index].price;

            invoice.items[invoice.itemCount].amount =
                quantity * products[index].price;

            invoice.itemCount++;
        }
    }

    if (invoice.itemCount == 0) {
        printf("Hoa don rong!\n");
        return;
    }

    for (i = 0; i < invoice.itemCount; i++) {
        index = findProduct(invoice.items[i].productId);

        invoice.total += invoice.items[i].amount;
        products[index].quantity -= invoice.items[i].quantity;
    }

    if (invoice.total >= 500000)
        invoice.discount = invoice.total * 0.10;

    invoice.finalTotal = invoice.total - invoice.discount;

    invoices[invoiceCount] = invoice;
    invoiceCount++;

    revenue += invoice.finalTotal;

    printf("\n========== HOA DON ==========\n");
    printf("%-12s %-20s %-8s %-12s %-12s\n",
           "Ma", "Ten", "SL", "Don gia", "Thanh tien");

    for (i = 0; i < invoice.itemCount; i++) {
        printf("%-12s %-20s %-8d %-12.2lf %-12.2lf\n",
               invoice.items[i].productId,
               invoice.items[i].productName,
               invoice.items[i].quantity,
               invoice.items[i].price,
               invoice.items[i].amount);
    }

    printf("Tong tien hang: %.2lf VND\n", invoice.total);
    printf("Giam gia: %.2lf VND\n", invoice.discount);
    printf("Khach phai tra: %.2lf VND\n", invoice.finalTotal);
    printf("=============================\n");
}

// 7. San pham sap het hang
void displayLowStock() {
    int i, count = 0;

    printf("\nDanh sach san pham sap het hang:\n");

    for (i = 0; i < productCount; i++) {
        if (products[i].quantity < 5) {
            printf("%s - %s - Con %d\n",
                   products[i].id,
                   products[i].name,
                   products[i].quantity);
            count++;
        }
    }

    if (count == 0)
        printf("Khong co san pham sap het hang!\n");
}

// 8. Sap xep don gia tang dan, neu bang nhau thi theo ma
void sortProducts() {
    int i, j;
    Product temp;

    for (i = 0; i < productCount - 1; i++) {
        for (j = i + 1; j < productCount; j++) {
            if (products[i].price > products[j].price ||
                (products[i].price == products[j].price &&
                 strcmp(products[i].id, products[j].id) > 0)) {

                temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }

    printf("Da sap xep thanh cong!\n");
    displayProducts();
}

// 9. Thong ke
void statistics() {
    int i, maxIndex = -1;
    double totalStockValue = 0;

    for (i = 0; i < productCount; i++) {
        totalStockValue += products[i].price * products[i].quantity;

        if (maxIndex == -1 ||
            products[i].price > products[maxIndex].price) {
            maxIndex = i;
        }
    }

    printf("\n========== THONG KE ==========\n");
    printf("So mat hang: %d\n", productCount);
    printf("Tong gia tri ton kho: %.2lf VND\n", totalStockValue);

    if (maxIndex != -1) {
        printf("San pham gia cao nhat: %s - %s\n",
               products[maxIndex].id,
               products[maxIndex].name);

        printf("Don gia cao nhat: %.2lf VND\n",
               products[maxIndex].price);
    } else {
        printf("Chua co san pham!\n");
    }

    printf("So hoa don da ban: %d\n", invoiceCount);
    printf("Tong doanh thu thuc thu: %.2lf VND\n", revenue);
    printf("===============================\n");
}

int main() {
    int choice;

    do {
        printf("\n====== QUAN LY CUA HANG ======\n");
        printf("1. Them san pham\n");
        printf("2. Hien thi danh sach san pham\n");
        printf("3. Tim san pham theo ma\n");
        printf("4. Cap nhat gia / nhap them hang\n");
        printf("5. Xoa san pham\n");
        printf("6. Ban hang va in hoa don\n");
        printf("7. Danh sach san pham sap het hang\n");
        printf("8. Sap xep theo don gia tang dan\n");
        printf("9. Thong ke\n");
        printf("0. Thoat\n");
        printf("==============================\n");

        printf("Chon: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayProducts();
                break;
            case 3:
                searchProduct();
                break;
            case 4:
                updateProduct();
                break;
            case 5:
                deleteProduct();
                break;
            case 6:
                sellProducts();
                break;
            case 7:
                displayLowStock();
                break;
            case 8:
                sortProducts();
                break;
            case 9:
                statistics();
                break;
            case 0:
                printf("Thoat chuong trinh!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }

    } while (choice != 0);

    return 0;
}
